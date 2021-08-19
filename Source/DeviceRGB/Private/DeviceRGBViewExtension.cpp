// Copyright(c) 2021 Viktor Pramberg
#include "DeviceRGBViewExtension.h"
#include "DeviceRGBSubsystem.h"
#include "IDeviceRGB.h"

#include <PostProcess/PostProcessMaterial.h>
#include <PostProcess/PostProcessing.h>
#include <ShaderParameterStruct.h>
#include <RHIDefinitions.h>
#include <MaterialShader.h>
#include <EngineModule.h>
#include <Async/Async.h>
#include <ScreenPass.h>
#include <Shader.h>

BEGIN_SHADER_PARAMETER_STRUCT(FDeviceRGBMaterialParameters, )
	SHADER_PARAMETER_STRUCT_REF(FViewUniformShaderParameters, View)
	SHADER_PARAMETER_STRUCT_INCLUDE(FSceneTextureShaderParameters, SceneTextures)
	SHADER_PARAMETER_STRUCT_ARRAY(FScreenPassTextureInput, PostProcessInput, [kPostProcessMaterialInputCountMax])
	SHADER_PARAMETER_STRUCT(FScreenPassTextureViewportParameters, PostProcessOutput) // This is needed for the material to compile in the Post Process domain
	SHADER_PARAMETER_SAMPLER(SamplerState, PostProcessInput_BilinearSampler)
	SHADER_PARAMETER_RDG_BUFFER_SRV(StructuredBuffer<float2>, LedUVs)
	SHADER_PARAMETER_RDG_BUFFER_SRV(StructuredBuffer<uint>, IndexBuffer)
	SHADER_PARAMETER_RDG_BUFFER_UAV(RWStructuredBuffer<float4>, Colors)
	SHADER_PARAMETER(uint32, IndexBufferStart)
	SHADER_PARAMETER(uint32, NumItems)
END_SHADER_PARAMETER_STRUCT()

class FDeviceRGBMaterialCS : public FMaterialShader
{
public:
	using FParameters = FDeviceRGBMaterialParameters;
	SHADER_USE_PARAMETER_STRUCT_WITH_LEGACY_BASE(FDeviceRGBMaterialCS, FMaterialShader);

	DECLARE_SHADER_TYPE(FDeviceRGBMaterialCS, Material);

	class FBlendModeDimension : SHADER_PERMUTATION_ENUM_CLASS("DEVICERGB_BLEND_MODE", EDeviceRGBBlendMode);
	class FIndexBufferDimension : SHADER_PERMUTATION_BOOL("DEVICERGB_INDEX_BUFFER");
	using FPermutationDomain = TShaderPermutationDomain<FBlendModeDimension, FIndexBufferDimension>;

	static constexpr uint32 ThreadGroupSize = 64; 

	static bool ShouldCompilePermutation(const FMaterialShaderPermutationParameters& Parameters)
	{
		if (Parameters.MaterialParameters.MaterialDomain == MD_PostProcess)
		{
			return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
		}
		return false;
	}

	static void ModifyCompilationEnvironment(const FMaterialShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& OutEnvironment)
	{
		FMaterialShader::ModifyCompilationEnvironment(Parameters, OutEnvironment);

		OutEnvironment.SetDefine(TEXT("POST_PROCESS_MATERIAL"), 1);

		OutEnvironment.SetDefine(TEXT("THREADGROUPSIZE_X"), ThreadGroupSize);
		OutEnvironment.SetDefine(TEXT("THREADGROUPSIZE_Y"), 1);
		OutEnvironment.SetDefine(TEXT("THREADGROUPSIZE_Z"), 1);
	}

	static void SetParameters(FRHICommandList& RHICmdList, const TShaderRef<FDeviceRGBMaterialCS>& Shader, const FViewInfo& View, const FMaterialRenderProxy* Proxy, const FParameters& Parameters)
	{
		FMaterialShader* MaterialShader = Shader.GetShader();
		MaterialShader->SetParameters(RHICmdList, Shader.GetComputeShader(), Proxy, *Proxy->GetMaterial(View.GetFeatureLevel()), View);
	}

	static void AddPass(FRDGBuilder& GraphBuilder, FRDGEventName&& Name, const TShaderRef<FDeviceRGBMaterialCS>& Shader,
						FParameters* Parameters, const FMaterialRenderProxy* Proxy, const FViewInfo& ViewInfo)
	{
		ClearUnusedGraphResources(Shader, Parameters);

		GraphBuilder.AddPass(MoveTemp(Name), Parameters, ERDGPassFlags::Compute, [&ViewInfo, Shader, Parameters, Proxy](FRHICommandList& RHICmdList)
		{
			FDeviceRGBMaterialCS::SetParameters(RHICmdList, Shader, ViewInfo, Proxy, *Parameters);
			FComputeShaderUtils::Dispatch(RHICmdList, Shader, *Parameters, FComputeShaderUtils::GetGroupCount(Parameters->NumItems, ThreadGroupSize));
		});
	}
};

IMPLEMENT_SHADER_TYPE(, FDeviceRGBMaterialCS, TEXT("/DeviceRGB/DeviceRGBMaterialCS.usf"), TEXT("MainCS"), SF_Compute);

FDeviceRGBSceneViewExtension::FDeviceRGBSceneViewExtension(const FAutoRegister& AutoRegister, UDeviceRGBSubsystem* InDeviceRGBSubsystem) : FSceneViewExtensionBase(AutoRegister), DeviceRGBSubsystem(InDeviceRGBSubsystem)
{
	GetRendererModule().RegisterPersistentViewUniformBufferExtension(this);
}

struct FDeviceRGBMaterialInfo
{
	const FMaterialRenderProxy* MaterialProxy = nullptr;
	const FMaterialShaderMap* ShaderMap = nullptr;
};

FDeviceRGBMaterialInfo GetMaterialInfo(const UMaterialInterface* InMaterialInterface, ERHIFeatureLevel::Type InFeatureLevel)
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

		return GetMaterialInfo(DefaultMaterial, InFeatureLevel);
	}

	const FMaterialShaderMap* MaterialShaderMap = Material->GetRenderingThreadShaderMap();;
	check(MaterialShaderMap);

	return { MaterialProxy, MaterialShaderMap };
}

bool FDeviceRGBSceneViewExtension::IsActiveThisFrameInContext(FSceneViewExtensionContext& Context) const
{
	if (!DeviceRGBSubsystem || !DeviceRGBSubsystem->IsEnabled())
	{
		return false;
	}

	if (DeviceRGBSubsystem->GetCachedInfos().Num() == 0 || DeviceRGBSubsystem->GetGraphicsCaches().Num() == 0)
	{
		return false;
	}

	for (auto& Cache : DeviceRGBSubsystem->GetGraphicsCaches())
	{
		if (!Cache.Material)
		{
			return false;
		}
	}

#if WITH_EDITOR
	if (Context.Viewport && (Context.Viewport->HasFocus() || CurrentActiveViewport == nullptr || CurrentActiveViewport->GetClient() == nullptr))
	{
		CurrentActiveViewport = Context.Viewport;
	}

	return CurrentActiveViewport == Context.Viewport;
#endif

	return true;
}

void FDeviceRGBSceneViewExtension::SubscribeToPostProcessingPass(EPostProcessingPass Pass, FAfterPassCallbackDelegateArray& InOutPassCallbacks, bool bIsPassEnabled)
{
	if (Pass == EPostProcessingPass::FXAA)
	{
		InOutPassCallbacks.Add(FAfterPassCallbackDelegate::CreateRaw(this, &FDeviceRGBSceneViewExtension::RenderLayers));
	}
}

FDeviceRGBMaterialParameters* FDeviceRGBSceneViewExtension::CreateParameters(FRDGBuilder& GraphBuilder, const FSceneView& View, const FPostProcessMaterialInputs& Inputs)
{
	RDG_EVENT_SCOPE(GraphBuilder, "DeviceRGB");

	FDeviceRGBMaterialParameters* Parameters = GraphBuilder.AllocParameters<FDeviceRGBMaterialCS::FParameters>();

	FScreenPassTexture SceneColor(Inputs.GetInput(EPostProcessMaterialInput::SceneColor));
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
	Parameters->PostProcessOutput = SceneTextureInput.Viewport;
	Parameters->PostProcessInput_BilinearSampler = TStaticSamplerState<SF_Bilinear, AM_Clamp, AM_Clamp, AM_Clamp>::GetRHI();;
	Parameters->SceneTextures = CreateSceneTextureShaderParameters(GraphBuilder, View.GetFeatureLevel(), ESceneTextureSetupMode::All);;
	Parameters->View = View.ViewUniformBuffer;

	FRDGBufferRef ColorsBuffer;
	if (!UVData.IsValid() || DeviceRGBSubsystem->bForceRefresh)
	{
		RDG_EVENT_SCOPE(GraphBuilder, "Setup");

		// UVs
		TArray<FVector2D> UVs;
		for (const auto& LedInfo : DeviceRGBSubsystem->GetCachedInfos())
		{
			UVs.Add(LedInfo.UV);
		}

		auto LedUVsBuffer = CreateStructuredBuffer(GraphBuilder, TEXT("DeviceRGB_LedUVs"), UVs.GetTypeSize(), UVs.Num(), UVs.GetData(), UVs.GetTypeSize() * UVs.Num());
		Parameters->LedUVs = GraphBuilder.CreateSRV(LedUVsBuffer);
		GraphBuilder.QueueBufferExtraction(LedUVsBuffer, &UVData, ERHIAccess::SRVCompute);

		// Index buffer
		TArray<uint32> Indices;
		for (auto& Graphic : DeviceRGBSubsystem->GetGraphicsCaches())
		{
			Indices.Append(Graphic.Indices);
		}

		if (Indices.Num() > 0)
		{
			auto IndexBuffer = CreateStructuredBuffer(GraphBuilder, TEXT("DeviceRGB_IndexBuffer"), Indices.GetTypeSize(), Indices.Num(), Indices.GetData(), Indices.GetTypeSize() * Indices.Num());
			Parameters->IndexBuffer = GraphBuilder.CreateSRV(IndexBuffer);
			GraphBuilder.QueueBufferExtraction(IndexBuffer, &IndexData, ERHIAccess::SRVCompute);
		}

		// Color
		TArray<FLinearColor> Colors;
		Colors.Init(FLinearColor::Black, UVs.Num());
		ColorsBuffer = CreateStructuredBuffer(GraphBuilder, TEXT("DeviceRGB_Colors"), Colors.GetTypeSize(), Colors.Num(), Colors.GetData(), Colors.GetTypeSize() * Colors.Num());
		Parameters->Colors = GraphBuilder.CreateUAV(ColorsBuffer);

		GraphBuilder.QueueBufferExtraction(ColorsBuffer, &ColorData, ERHIAccess::UAVCompute);

		DeviceRGBSubsystem->bForceRefresh = false;
	}
	else
	{
		if (UVData.IsValid())
		{
			Parameters->LedUVs = GraphBuilder.CreateSRV(GraphBuilder.RegisterExternalBuffer(UVData, ERDGBufferFlags::MultiFrame));
		}
		
		if (IndexData.IsValid())
		{
			Parameters->IndexBuffer = GraphBuilder.CreateSRV(GraphBuilder.RegisterExternalBuffer(IndexData, ERDGBufferFlags::MultiFrame));
		}

		ColorsBuffer = GraphBuilder.RegisterExternalBuffer(ColorData, ERDGBufferFlags::MultiFrame);
		Parameters->Colors = GraphBuilder.CreateUAV(ColorsBuffer);
	}

	GraphBuilder.QueueBufferExtraction(ColorsBuffer, &ReadbackData, ERHIAccess::CPURead);

	return Parameters;
}

FScreenPassTexture FDeviceRGBSceneViewExtension::RenderLayers(FRDGBuilder& GraphBuilder, const FSceneView& View, const FPostProcessMaterialInputs& Inputs)
{
	RDG_EVENT_SCOPE(GraphBuilder, "DeviceRGB");

	checkSlow(View.bIsViewInfo);
	const FViewInfo& ViewInfo = static_cast<const FViewInfo&>(View);

	Inputs.Validate();

	RDG_EVENT_SCOPE(GraphBuilder, "Layers");

	uint32 IndexBufferStart = 0;
	const auto& GraphicCaches = DeviceRGBSubsystem->GetGraphicsCaches();
	for (int32 LayerIndex = 0; LayerIndex < GraphicCaches.Num(); LayerIndex++)
	{
		FDeviceRGBMaterialCS::FParameters* Parameters = CreateParameters(GraphBuilder, View, Inputs);

		const auto& Cache = GraphicCaches[LayerIndex];
		const bool bHasIndexBuffer = Cache.Indices.Num() > 0;

		if (bHasIndexBuffer)
		{
			Parameters->IndexBufferStart = IndexBufferStart;
			Parameters->NumItems = Cache.Indices.Num();
			IndexBufferStart += Parameters->NumItems;
		}
		else
		{
			Parameters->NumItems = DeviceRGBSubsystem->GetCachedInfos().Num();
		}

		UMaterialInterface* MaterialInterface = Cache.Material.Get();
		const FDeviceRGBMaterialInfo& MaterialInfo = GetMaterialInfo(MaterialInterface, View.GetFeatureLevel());

		FDeviceRGBMaterialCS::FPermutationDomain PermutationVector;
		PermutationVector.Set<FDeviceRGBMaterialCS::FBlendModeDimension>(Cache.BlendMode);
		PermutationVector.Set<FDeviceRGBMaterialCS::FIndexBufferDimension>(bHasIndexBuffer);
		auto Shader = MaterialInfo.ShaderMap->GetShader<FDeviceRGBMaterialCS>(PermutationVector);

		FDeviceRGBMaterialCS::AddPass(
			GraphBuilder, RDG_EVENT_NAME("Layer %d - %s", LayerIndex, *MaterialInterface->GetName()),
			Shader, Parameters, MaterialInfo.MaterialProxy, ViewInfo
		);
	}

	return Inputs.GetInput(EPostProcessMaterialInput::SceneColor);
}

void FDeviceRGBSceneViewExtension::EndFrame()
{
	if (!DeviceRGBSubsystem || !DeviceRGBSubsystem->IsEnabled() || DeviceRGBSubsystem->GetGraphicsCaches().Num() == 0)
	{
		return;
	}

	ENQUEUE_RENDER_COMMAND(FPerformEndOfFrameReadback)([this](FRHICommandListImmediate& RHICmdList)
	{
		if (ReadbackData && ReadbackData->GetStructuredBufferRHI() && ReadbackData->GetStructuredBufferRHI()->IsValid())
		{
			SCOPED_DRAW_EVENT(RHICmdList, DeviceRGB_DataReadback);

			TArray<FLinearColor> ColorsTemp;
			ColorsTemp.Init(FLinearColor::Black, DeviceRGBSubsystem->GetCachedInfos().Num());

			const uint32 Size = static_cast<uint32>(ColorsTemp.Num()) * ColorsTemp.GetTypeSize();

			void* Source = RHICmdList.LockStructuredBuffer(ReadbackData->GetStructuredBufferRHI(), 0, Size, RLM_ReadOnly);
			FMemory::Memcpy(ColorsTemp.GetData(), Source, Size);
			RHICmdList.UnlockStructuredBuffer(ReadbackData->GetStructuredBufferRHI());
			
			AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, Colors = MoveTemp(ColorsTemp)]()
			{
				auto It = Colors.CreateConstIterator();
				DeviceRGBSubsystem->SetColors([&](IDeviceRGB* InDevice, TArray<FColor>& OutColors)
				{
					for (const auto& LEDInfo : InDevice->GetLEDInfos())
					{
						OutColors.Add(It->QuantizeRound());
						++It;
					}
				});
			});
		}
	});
}
