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
#include "IDeviceRGB.h"

BEGIN_SHADER_PARAMETER_STRUCT(FDeviceRGBMaterialParameters, )
	SHADER_PARAMETER_STRUCT_REF(FViewUniformShaderParameters, View)
	SHADER_PARAMETER_STRUCT_INCLUDE(FSceneTextureShaderParameters, SceneTextures)
	SHADER_PARAMETER_STRUCT_ARRAY(FScreenPassTextureInput, PostProcessInput, [kPostProcessMaterialInputCountMax])
	SHADER_PARAMETER_STRUCT(FScreenPassTextureViewportParameters, PostProcessOutput) // This is needed for the material to compile in the Post Process domain
	SHADER_PARAMETER_SAMPLER(SamplerState, PostProcessInput_BilinearSampler)
	SHADER_PARAMETER_RDG_BUFFER_SRV(StructuredBuffer<float2>, LedUVs)
	SHADER_PARAMETER_RDG_BUFFER_UAV(RWStructuredBuffer<float4>, Colors)
END_SHADER_PARAMETER_STRUCT()

class FDeviceRGBMaterialCS : public FMaterialShader
{
public:
	using FParameters = FDeviceRGBMaterialParameters;
	SHADER_USE_PARAMETER_STRUCT_WITH_LEGACY_BASE(FDeviceRGBMaterialCS, FMaterialShader);

	DECLARE_SHADER_TYPE(FDeviceRGBMaterialCS, Material);

	class FBlendModeDimension : SHADER_PERMUTATION_ENUM_CLASS("DEVICERGB_BLEND_MODE", EDeviceRGBBlendMode);
	using FPermutationDomain = TShaderPermutationDomain<FBlendModeDimension>;

	static void SetParameters(FRHICommandList& RHICmdList, const TShaderRef<FDeviceRGBMaterialCS>& Shader, const FViewInfo& View, const FMaterialRenderProxy* Proxy, const FParameters& Parameters)
	{
		FMaterialShader* MaterialShader = Shader.GetShader();
		MaterialShader->SetParameters(RHICmdList, Shader.GetComputeShader(), Proxy, *Proxy->GetMaterial(View.GetFeatureLevel()), View);
	}

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

		OutEnvironment.SetDefine(TEXT("THREADGROUPSIZE_X"), 64);
		OutEnvironment.SetDefine(TEXT("THREADGROUPSIZE_Y"), 1);
		OutEnvironment.SetDefine(TEXT("THREADGROUPSIZE_Z"), 1);
	}
};

IMPLEMENT_SHADER_TYPE(, FDeviceRGBMaterialCS, TEXT("/DeviceRGB/DeviceRGBMaterialCS.usf"), TEXT("MainCS"), SF_Compute);

FDeviceRGBSceneViewExtension::FDeviceRGBSceneViewExtension(const FAutoRegister& AutoRegister, UDeviceRGBSubsystem* InDeviceRGBSubsystem) : FSceneViewExtensionBase(AutoRegister), DeviceRGBSubsystem(InDeviceRGBSubsystem)
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

bool FDeviceRGBSceneViewExtension::IsActiveThisFrameInContext(FSceneViewExtensionContext& Context) const
{
	return DeviceRGBSubsystem &&
		DeviceRGBSubsystem->GetCurrentGraphic().IsType<UMaterialInterface*>() &&
		DeviceRGBSubsystem->GetCachedInfos().Num() > 0;
}

void FDeviceRGBSceneViewExtension::SubscribeToPostProcessingPass(EPostProcessingPass Pass, FAfterPassCallbackDelegateArray& InOutPassCallbacks, bool bIsPassEnabled)
{
	if (Pass == EPostProcessingPass::FXAA)
	{
		InOutPassCallbacks.Add(FAfterPassCallbackDelegate::CreateRaw(this, &FDeviceRGBSceneViewExtension::RenderLayers));
	}
}

FScreenPassTexture FDeviceRGBSceneViewExtension::RenderLayers(FRDGBuilder& GraphBuilder, const FSceneView& View, const FPostProcessMaterialInputs& Inputs)
{
	checkSlow(View.bIsViewInfo);

	Inputs.Validate();

	// This is safe, otherwise this function wouldn't execute
	UMaterialInterface* MaterialInterface = DeviceRGBSubsystem->GetCurrentGraphic().Get<UMaterialInterface*>();
	const FMaterialRenderProxy* Proxy = nullptr;
	const FMaterial* Material = nullptr;
	const FMaterialShaderMap* ShaderMap = nullptr;
	GetMaterialInfo(MaterialInterface, View.GetFeatureLevel(), Material, Proxy, ShaderMap);

	FScreenPassTexture SceneColor(Inputs.GetInput(EPostProcessMaterialInput::SceneColor));

	if (!ShaderMap)
		return SceneColor;

	FDeviceRGBMaterialCS::FParameters* Parameters = GraphBuilder.AllocParameters<FDeviceRGBMaterialCS::FParameters>();

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
	if (!UVData.IsValid() || DeviceRGBSubsystem->bForceRefresh)
	{
		RDG_EVENT_SCOPE(GraphBuilder, "Setup");

		TArray<FVector2D> UVs;
		for (const auto& LedInfo : DeviceRGBSubsystem->GetCachedInfos())
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

		DeviceRGBSubsystem->bForceRefresh = false;
	}
	else
	{
		ColorsBuffer = GraphBuilder.RegisterExternalBuffer(ColorData, ERDGBufferFlags::MultiFrame);
		Parameters->LedUVs = GraphBuilder.CreateSRV(GraphBuilder.RegisterExternalBuffer(UVData, ERDGBufferFlags::MultiFrame));
		Parameters->Colors = GraphBuilder.CreateUAV(ColorsBuffer);
	}

	const int32 NumItems = DeviceRGBSubsystem->GetCachedInfos().Num();

	FDeviceRGBMaterialCS::FPermutationDomain PermutationVector;
	PermutationVector.Set<FDeviceRGBMaterialCS::FBlendModeDimension>(EDeviceRGBBlendMode::AlphaBlend);
	auto Shader = ShaderMap->GetShader<FDeviceRGBMaterialCS>(PermutationVector);

	RDG_EVENT_SCOPE(GraphBuilder, "Layers");

	ClearUnusedGraphResources(Shader, Parameters);

	GraphBuilder.AddPass(RDG_EVENT_NAME("Layer 0 - %s", *MaterialInterface->GetName()), Parameters,
		ERDGPassFlags::Compute, [&View, Shader, Proxy, Parameters, NumItems](FRHICommandList& RHICmdList)
	{
		FDeviceRGBMaterialCS::SetParameters(RHICmdList, Shader, static_cast<const FViewInfo&>(View), Proxy, *Parameters);
		FComputeShaderUtils::Dispatch(RHICmdList, Shader, *Parameters, FComputeShaderUtils::GetGroupCount(NumItems, 64));
	});

	GraphBuilder.QueueBufferExtraction(ColorsBuffer, &ReadbackData, ERHIAccess::CPURead);

	return SceneColor;
}

void FDeviceRGBSceneViewExtension::EndFrame()
{
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
						OutColors.Add((*It).QuantizeRound());
						++It;
					}
				});
			});
		}
	});
}
