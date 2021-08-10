// Copyright(c) 2021 Viktor Pramberg
#include "DeviceRGBViewExtension.h"
#include "DeviceRGBSubsystem.h"
#include <MaterialShader.h>
#include <ScreenPass.h>
#include <PostProcess/PostProcessMaterial.h>
#include <ShaderParameterStruct.h>
#include <Shader.h>
#include <RHIDefinitions.h>
#include <PostProcess/PostProcessing.h>
#include <EngineModule.h>
#include "IDevice.h"

BEGIN_SHADER_PARAMETER_STRUCT(FDeviceRGBMaterialParameters, )
	SHADER_PARAMETER_STRUCT_REF(FViewUniformShaderParameters, View)
	SHADER_PARAMETER_STRUCT_INCLUDE(FSceneTextureShaderParameters, SceneTextures)
	SHADER_PARAMETER_STRUCT_ARRAY(FScreenPassTextureInput, PostProcessInput, [kPostProcessMaterialInputCountMax])
	SHADER_PARAMETER_STRUCT(FScreenPassTextureViewportParameters, PostProcessOutput) // This is needed for the material to compile in the Post Process domain
	SHADER_PARAMETER_SAMPLER(SamplerState, PostProcessInput_BilinearSampler)
	SHADER_PARAMETER_RDG_BUFFER_SRV(StructuredBuffer<float2>, LedUVs)
	SHADER_PARAMETER_RDG_BUFFER_UAV(RWStructuredBuffer<float4>, Colors)
END_SHADER_PARAMETER_STRUCT()

class FDeviceRGBMaterialShader : public FMaterialShader
{
public:
	using FParameters = FDeviceRGBMaterialParameters;
	SHADER_USE_PARAMETER_STRUCT_WITH_LEGACY_BASE(FDeviceRGBMaterialShader, FMaterialShader);

	class FMobileDimension : SHADER_PERMUTATION_BOOL("POST_PROCESS_MATERIAL_MOBILE");
	using FPermutationDomain = TShaderPermutationDomain<FMobileDimension>;

	static bool ShouldCompilePermutation(const FMaterialShaderPermutationParameters& Parameters)
	{
		if (Parameters.MaterialParameters.MaterialDomain == MD_PostProcess)
		{
			const FPermutationDomain PermutationVector(Parameters.PermutationId);

			if (PermutationVector.Get<FMobileDimension>())
			{
				return IsMobilePlatform(Parameters.Platform) && IsMobileHDR();
			}
			else
			{
				return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
			}
		}
		return false;
	}

	static void ModifyCompilationEnvironment(const FMaterialShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& OutEnvironment)
	{
		FMaterialShader::ModifyCompilationEnvironment(Parameters, OutEnvironment);
		OutEnvironment.SetDefine(TEXT("POST_PROCESS_MATERIAL"), 1);

		EBlendableLocation Location = EBlendableLocation(Parameters.MaterialParameters.BlendableLocation);
		OutEnvironment.SetDefine(TEXT("POST_PROCESS_MATERIAL_BEFORE_TONEMAP"), (Location == BL_AfterTonemapping || Location == BL_ReplacingTonemapper) ? 0 : 1);
		
		FPermutationDomain PermutationVector(Parameters.PermutationId);
		if (PermutationVector.Get<FMobileDimension>())
		{
			OutEnvironment.SetDefine(TEXT("POST_PROCESS_MATERIAL_BEFORE_TONEMAP"), (Parameters.MaterialParameters.BlendableLocation != BL_AfterTonemapping) ? 1 : 0);
		}
	}

protected:
	template <typename TRHIShader>
	static void SetParameters(FRHICommandList& RHICmdList, const TShaderRef<FDeviceRGBMaterialShader>& Shader, TRHIShader* ShaderRHI, const FViewInfo& View, const FMaterialRenderProxy* Proxy, const FParameters& Parameters)
	{
		FMaterialShader* MaterialShader = Shader.GetShader();
		MaterialShader->SetParameters(RHICmdList, ShaderRHI, Proxy, *Proxy->GetMaterial(View.GetFeatureLevel()), View);
		SetShaderParameters(RHICmdList, Shader, ShaderRHI, Parameters);
	}
};

class FDeviceRGBMaterialCS : public FDeviceRGBMaterialShader
{
public:
	DECLARE_SHADER_TYPE(FDeviceRGBMaterialCS, Material);

	static void SetParameters(FRHICommandList& RHICmdList, const TShaderRef<FDeviceRGBMaterialCS>& Shader, const FViewInfo& View, const FMaterialRenderProxy* Proxy, const FParameters& Parameters)
	{
		FDeviceRGBMaterialShader::SetParameters(RHICmdList, Shader, Shader.GetPixelShader(), View, Proxy, Parameters);
	}

	FDeviceRGBMaterialCS() = default;
	FDeviceRGBMaterialCS(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
		: FDeviceRGBMaterialShader(Initializer)
	{}

	static void ModifyCompilationEnvironment(const FMaterialShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& OutEnvironment)
	{
		FDeviceRGBMaterialShader::ModifyCompilationEnvironment(Parameters, OutEnvironment);

		uint32 StencilCompareFunction = Parameters.MaterialParameters.bIsStencilTestEnabled ? Parameters.MaterialParameters.StencilCompare : EMaterialStencilCompare::MSC_Never;

		OutEnvironment.SetDefine(TEXT("MOBILE_STENCIL_COMPARE_FUNCTION"), StencilCompareFunction);

		OutEnvironment.SetDefine(TEXT("THREADGROUPSIZE_X"), 64);
		OutEnvironment.SetDefine(TEXT("THREADGROUPSIZE_Y"), 1);
		OutEnvironment.SetDefine(TEXT("THREADGROUPSIZE_Z"), 1);
	}
};

IMPLEMENT_SHADER_TYPE(, FDeviceRGBMaterialCS, TEXT("/DeviceRGB/DeviceRGBMaterialCS.usf"), TEXT("MainCS"), SF_Compute);

FDeviceRGBSceneViewExtension::FDeviceRGBSceneViewExtension(const FAutoRegister& AutoRegister, UDeviceRGBSubsystem* InEngineSubsystem) : FSceneViewExtensionBase(AutoRegister), EngineSubsystem(InEngineSubsystem)
{
	GetRendererModule().RegisterPersistentViewUniformBufferExtension(this);
}

void GetMaterialInfo(
	const UMaterialInterface* InMaterialInterface,
	ERHIFeatureLevel::Type InFeatureLevel,
	const FMaterial*& OutMaterial,
	const FMaterialRenderProxy*& OutMaterialProxy,
	const FMaterialShaderMap*& OutMaterialShaderMap)
{
	const FMaterialRenderProxy* MaterialProxy = InMaterialInterface->GetRenderProxy();
	check(MaterialProxy);
	
	const FMaterial* Material = MaterialProxy->GetMaterialNoFallback(InFeatureLevel);

	if (!Material || Material->GetMaterialDomain() != MD_PostProcess || !Material->GetRenderingThreadShaderMap())
	{
		// Fallback to the default post process material.
		const UMaterialInterface* DefaultMaterial = UMaterial::GetDefaultMaterial(MD_PostProcess);
		check(DefaultMaterial);
		check(DefaultMaterial != InMaterialInterface);

		return GetMaterialInfo(
			DefaultMaterial,
			InFeatureLevel,
			OutMaterial,
			OutMaterialProxy,
			OutMaterialShaderMap);
	}

	const FMaterialShaderMap* MaterialShaderMap = Material->GetRenderingThreadShaderMap();;
	check(MaterialShaderMap);

	OutMaterial = Material;
	OutMaterialProxy = MaterialProxy;
	OutMaterialShaderMap = MaterialShaderMap;
}

BEGIN_SHADER_PARAMETER_STRUCT(FDeviceRGBReadbackBufferParameters, )
RDG_BUFFER_ACCESS(Buffer, ERHIAccess::CopySrc)
END_SHADER_PARAMETER_STRUCT()

template <typename ExecuteLambdaType>
void AddReadbackBufferPass(FRDGBuilder& GraphBuilder, FRDGEventName&& Name, FRDGBufferRef Buffer, ExecuteLambdaType&& ExecuteLambda)
{
	auto* PassParameters = GraphBuilder.AllocParameters<FDeviceRGBReadbackBufferParameters>();
	PassParameters->Buffer = Buffer;
	GraphBuilder.AddPass(MoveTemp(Name), PassParameters, ERDGPassFlags::Readback, MoveTemp(ExecuteLambda));
}

void FDeviceRGBSceneViewExtension::PrePostProcessPass_RenderThread(FRDGBuilder& GraphBuilder, const FSceneView& View, const FPostProcessingInputs& Inputs)
{
	checkSlow(View.bIsViewInfo);

	Inputs.Validate();

	// This is safe, otherwise this function wouldn't execute
	UMaterialInterface* MaterialInterface = EngineSubsystem->GetCurrentGraphic().Get<UMaterialInterface*>();
	const FMaterialRenderProxy* Proxy = nullptr;
	const FMaterial* Material = nullptr;
	const FMaterialShaderMap* ShaderMap = nullptr;
	GetMaterialInfo(MaterialInterface, View.GetFeatureLevel(), Material, Proxy, ShaderMap);
	
	if (!ShaderMap)
		return;

	FDeviceRGBMaterialCS::FParameters* Parameters = GraphBuilder.AllocParameters<FDeviceRGBMaterialCS::FParameters>();

	FScreenPassTexture SceneColor((*Inputs.SceneTextures)->SceneColorTexture);
	const FScreenPassTextureViewport SceneColorTextureViewport(SceneColor);
	FScreenPassRenderTarget SceneColorRenderTarget(SceneColor, ERenderTargetLoadAction::ELoad);
	FRHISamplerState* PointClampSampler = TStaticSamplerState<SF_Point, AM_Clamp, AM_Clamp, AM_Clamp>::GetRHI();

	FScreenPassTextureInput SceneTextureInput;
	{
		SceneTextureInput.Viewport = GetScreenPassTextureViewportParameters(SceneColorTextureViewport);
		SceneTextureInput.Texture = SceneColorRenderTarget.Texture;
		SceneTextureInput.Sampler = PointClampSampler;
	}

	for (uint32 InputIndex = 0; InputIndex < kPostProcessMaterialInputCountMax; ++InputIndex)
	{
		Parameters->PostProcessInput[InputIndex] = SceneTextureInput;
	}
	Parameters->PostProcessOutput = GetScreenPassTextureViewportParameters(SceneColorTextureViewport);
	Parameters->PostProcessInput_BilinearSampler = TStaticSamplerState<SF_Bilinear, AM_Clamp, AM_Clamp, AM_Clamp>::GetRHI();;
	Parameters->SceneTextures = CreateSceneTextureShaderParameters(GraphBuilder, View.GetFeatureLevel(), ESceneTextureSetupMode::All);;
	Parameters->View = View.ViewUniformBuffer;

	RDG_EVENT_SCOPE(GraphBuilder, "DeviceRGB");

	FRDGBufferRef ColorsBuffer;
	if (!UVData.IsValid())
	{
		RDG_EVENT_SCOPE(GraphBuilder, "Setup");
		
		TArray<FVector2D> UVs;
		for (const auto& LedInfo : EngineSubsystem->GetCachedInfos())
		{
			UVs.Add(LedInfo.UV);
		}

		auto LedUVsBuffer = CreateStructuredBuffer(GraphBuilder, TEXT("DeviceRGB_LedUVs"), UVs.GetTypeSize(), UVs.Num(), UVs.GetData(), UVs.GetTypeSize() * UVs.Num());
		Parameters->LedUVs = GraphBuilder.CreateSRV(LedUVsBuffer);
		GraphBuilder.QueueBufferExtraction(LedUVsBuffer, &UVData, ERHIAccess::UAVCompute);

		TArray<FLinearColor> Colors;
		Colors.Init(FLinearColor::Black, UVs.Num());
		ColorsBuffer = CreateStructuredBuffer(GraphBuilder, TEXT("DeviceRGB_Colors"), Colors.GetTypeSize(), Colors.Num(), Colors.GetData(), Colors.GetTypeSize() * Colors.Num());
		Parameters->Colors = GraphBuilder.CreateUAV(ColorsBuffer);

		GraphBuilder.QueueBufferExtraction(ColorsBuffer, &ColorData, ERHIAccess::UAVCompute);
	}
	else
	{
		ColorsBuffer = GraphBuilder.RegisterExternalBuffer(ColorData, ERDGBufferFlags::MultiFrame);
		Parameters->LedUVs = GraphBuilder.CreateSRV(GraphBuilder.RegisterExternalBuffer(UVData, ERDGBufferFlags::MultiFrame));
		Parameters->Colors = GraphBuilder.CreateUAV(ColorsBuffer);
	}
	
	const bool bIsMobile = View.GetFeatureLevel() <= ERHIFeatureLevel::ES3_1;
	const int32 NumItems = EngineSubsystem->GetCachedInfos().Num();
	FDeviceRGBMaterialCS::FPermutationDomain PermutationVector;
	PermutationVector.Set<FDeviceRGBMaterialCS::FMobileDimension>(bIsMobile);

	
	auto Shader = ShaderMap->GetShader<FDeviceRGBMaterialCS>(PermutationVector);
	
	RDG_EVENT_SCOPE(GraphBuilder, "Layers");

	ClearUnusedGraphResources(Shader, Parameters);

	GraphBuilder.AddPass(RDG_EVENT_NAME("Layer 0 - %s", *MaterialInterface->GetName()), Parameters, 
		ERDGPassFlags::Compute, [&View, Shader, Proxy, Parameters, NumItems](FRHICommandList& RHICmdList)
	{
		FMaterialShader* MaterialShader = Shader.GetShader();
		MaterialShader->SetParameters(RHICmdList, Shader.GetComputeShader(), Proxy, *Proxy->GetMaterial(View.GetFeatureLevel()), static_cast<const FViewInfo&>(View));

		FComputeShaderUtils::Dispatch(RHICmdList, Shader, *Parameters, FComputeShaderUtils::GetGroupCount(NumItems, 64));
	});

	GraphBuilder.QueueBufferExtraction(ColorsBuffer, &Data, ERHIAccess::CPURead);
}

void FDeviceRGBSceneViewExtension::SetupViewFamily(FSceneViewFamily& InViewFamily)
{
}

void FDeviceRGBSceneViewExtension::SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView)
{
}

void FDeviceRGBSceneViewExtension::BeginRenderViewFamily(FSceneViewFamily& InViewFamily)
{
}

void FDeviceRGBSceneViewExtension::PreRenderViewFamily_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneViewFamily& InViewFamily)
{
}

void FDeviceRGBSceneViewExtension::PreRenderView_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneView& InView)
{
}

void FDeviceRGBSceneViewExtension::PostRenderViewFamily_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneViewFamily& InViewFamily)
{
}

bool FDeviceRGBSceneViewExtension::IsActiveThisFrameInContext(FSceneViewExtensionContext& Context) const
{
	return EngineSubsystem->GetCurrentGraphic().IsType<UMaterialInterface*>();
}

void FDeviceRGBSceneViewExtension::PostRenderView_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneView& InView)
{
}

void FDeviceRGBSceneViewExtension::EndFrame()
{
	ENQUEUE_RENDER_COMMAND(FPerformEndOfFrameReadback)([this](FRHICommandListImmediate& RHICmdList)
	{
		if (Data && Data->GetStructuredBufferRHI() && Data->GetStructuredBufferRHI()->IsValid())
		{
			SCOPED_DRAW_EVENT(RHICmdList, DeviceRGB_DataReadback);
			TArray<FLinearColor> ColorsTemp;
			ColorsTemp.Init(FLinearColor::Black, EngineSubsystem->GetCachedInfos().Num());

			const uint32 Size = static_cast<uint32>(ColorsTemp.Num()) * ColorsTemp.GetTypeSize();

			void* Source = RHICmdList.LockStructuredBuffer(Data->GetStructuredBufferRHI(), 0, Size, RLM_ReadOnly);
			FMemory::Memcpy(ColorsTemp.GetData(), Source, Size);
			RHICmdList.UnlockStructuredBuffer(Data->GetStructuredBufferRHI());
			
			AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, Colors = MoveTemp(ColorsTemp)]()
			{
				auto It = Colors.CreateConstIterator();
				for (auto& SDK : EngineSubsystem->SupportedSDKs)
				{
					SDK->SetColors([&](IDevice* InDevice, TArray<FColor>& OutColors)
					{
						for (const auto& LEDInfo : InDevice->GetLEDInfos())
						{
							// I am still on the fence whether to convert to sRGB or not? When converting it gets brighter, which is a good thing generally.
							OutColors.Add((*It).ToFColor(true));
							++It;
						}
					});
				}
			});
		}
	});
}
