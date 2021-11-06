// Copyright(c) 2021 Viktor Pramberg
#include "DeviceRGBViewExtension.h"
#include "DeviceRGBSubsystem.h"
#include "IDeviceRGB.h"
#include "DeviceRGBStats.h"

#include <PostProcess/PostProcessMaterial.h>
#include <PostProcess/PostProcessing.h>
#include <ShaderParameterStruct.h>
#include <RHIDefinitions.h>
#include <MaterialShader.h>
#include <EngineModule.h>
#include <Async/Async.h>
#include <ScreenPass.h>
#include <Shader.h>

DECLARE_CYCLE_STAT(TEXT("Gather Frame Resources"), STAT_GatherFrameResources, STATGROUP_DeviceRGB);
DECLARE_CYCLE_STAT(TEXT("Dispatch Layers"), STAT_DispatchLayers, STATGROUP_DeviceRGB);
DECLARE_CYCLE_STAT(TEXT("End Frame"), STAT_EndFrame, STATGROUP_DeviceRGB);
DECLARE_CYCLE_STAT(TEXT("Readback"), STAT_Readback, STATGROUP_DeviceRGB);
DECLARE_CYCLE_STAT(TEXT("Readback Lock"), STAT_ReadbackLock, STATGROUP_DeviceRGB);
DECLARE_CYCLE_STAT(TEXT("Set Colors"), STAT_SetColors, STATGROUP_DeviceRGB);

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

	if (!DeviceRGBSubsystem->ShouldDoCompleteRebuild())
	{
		if (!UVData.IsValid() || !ColorData.IsValid())
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
		InOutPassCallbacks.Add(FAfterPassCallbackDelegate::CreateRaw(this, &FDeviceRGBSceneViewExtension::DispatchLayers));
	}
}

FDeviceRGBMaterialParameters* FDeviceRGBSceneViewExtension::CreateParameters(FRDGBuilder& GraphBuilder, const FSceneView& View, const FPostProcessMaterialInputs& Inputs, const FDeviceRGBFrameResources& FrameResources)
{
	RDG_EVENT_SCOPE(GraphBuilder, "DeviceRGB");

	FDeviceRGBMaterialParameters* Parameters = GraphBuilder.AllocParameters<FDeviceRGBMaterialCS::FParameters>();

	const FScreenPassTexture SceneColor(Inputs.GetInput(EPostProcessMaterialInput::SceneColor));
	const FScreenPassTextureViewport SceneColorTextureViewport(SceneColor);
	const FScreenPassRenderTarget SceneColorRenderTarget(SceneColor, ERenderTargetLoadAction::ELoad);
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

	Parameters->LedUVs = FrameResources.UV_SRV;
	Parameters->Colors = FrameResources.Colors_UAV;
	Parameters->IndexBuffer = FrameResources.IndexBuffer_SRV;

	return Parameters;
}

FDeviceRGBFrameResources FDeviceRGBSceneViewExtension::GatherFrameResources(FRDGBuilder& GraphBuilder)
{
	SCOPE_CYCLE_COUNTER(STAT_GatherFrameResources);

	FDeviceRGBFrameResources FrameResources;

	if (DeviceRGBSubsystem->ShouldRebuild())
	{
		RDG_EVENT_SCOPE(GraphBuilder, "Setup");

		FScopeLock Lock(&DeviceRGBSubsystem->DeviceInfoCriticalSection);

		// Index buffer - Will always need to be rebuilt
		TArray<uint32> Indices;
		for (auto& Graphic : DeviceRGBSubsystem->GetGraphicsCaches())
		{
			Indices.Append(Graphic.Indices);
		}

		const auto CreateStructuredBufferFromArray = [&](const TCHAR* Name, const auto& Array)
		{
			return CreateStructuredBuffer(GraphBuilder, Name, Array.GetTypeSize(), Array.Num(), Array.GetData(), Array.GetTypeSize() * Array.Num());
		};

		if (Indices.Num() > 0)
		{
			auto IndexBuffer = CreateStructuredBufferFromArray(TEXT("DeviceRGB_IndexBuffer"), Indices);
			FrameResources.IndexBuffer_SRV = GraphBuilder.CreateSRV(IndexBuffer);
			GraphBuilder.QueueBufferExtraction(IndexBuffer, &IndexData, ERHIAccess::SRVCompute);
		}
		else
		{
			IndexData.SafeRelease();
		}

		if (DeviceRGBSubsystem->ShouldDoCompleteRebuild())
		{
			TArray<FVector2D> UVs;
			for (const auto& LedInfo : DeviceRGBSubsystem->GetCachedInfos())
			{
				UVs.Add(LedInfo.UV);
			}
			NumLeds = UVs.Num();

			auto LedUVsBuffer = CreateStructuredBufferFromArray(TEXT("DeviceRGB_LedUVs"), UVs);
			FrameResources.UV_SRV = GraphBuilder.CreateSRV(LedUVsBuffer);
			GraphBuilder.QueueBufferExtraction(LedUVsBuffer, &UVData, ERHIAccess::SRVCompute);

			// Color
			TArray<FLinearColor> Colors;
			Colors.Init(FLinearColor::Black, NumLeds);
			FrameResources.ColorBuffer = CreateStructuredBufferFromArray(TEXT("DeviceRGB_Colors"), Colors);
			FrameResources.Colors_UAV = GraphBuilder.CreateUAV(FrameResources.ColorBuffer);

			GraphBuilder.QueueBufferExtraction(FrameResources.ColorBuffer, &ColorData, ERHIAccess::CPURead);
		}
		else
		{
			if (UVData.IsValid())
			{
				FrameResources.UV_SRV = GraphBuilder.CreateSRV(GraphBuilder.RegisterExternalBuffer(UVData, ERDGBufferFlags::MultiFrame));
			}

			FrameResources.ColorBuffer = GraphBuilder.RegisterExternalBuffer(ColorData, ERDGBufferFlags::MultiFrame);
			FrameResources.Colors_UAV = GraphBuilder.CreateUAV(FrameResources.ColorBuffer);
		}

		DeviceRGBSubsystem->ResetDirtyFlags();
	}
	else
	{
		check(UVData.IsValid() && ColorData.IsValid());

		FrameResources.UV_SRV = GraphBuilder.CreateSRV(GraphBuilder.RegisterExternalBuffer(UVData, ERDGBufferFlags::MultiFrame));
		FrameResources.ColorBuffer = GraphBuilder.RegisterExternalBuffer(ColorData, ERDGBufferFlags::MultiFrame);
		FrameResources.Colors_UAV = GraphBuilder.CreateUAV(FrameResources.ColorBuffer);

		if (IndexData.IsValid())
		{
			FrameResources.IndexBuffer_SRV = GraphBuilder.CreateSRV(GraphBuilder.RegisterExternalBuffer(IndexData, ERDGBufferFlags::MultiFrame));
		}
	}

	return FrameResources;
}

FScreenPassTexture FDeviceRGBSceneViewExtension::DispatchLayers(FRDGBuilder& GraphBuilder, const FSceneView& View, const FPostProcessMaterialInputs& Inputs)
{
	SCOPE_CYCLE_COUNTER(STAT_DispatchLayers);

	RDG_EVENT_SCOPE(GraphBuilder, "DeviceRGB");

	checkSlow(View.bIsViewInfo);
	const FViewInfo& ViewInfo = static_cast<const FViewInfo&>(View);

	Inputs.Validate();

	const FDeviceRGBFrameResources FrameResources = GatherFrameResources(GraphBuilder);

	RDG_EVENT_SCOPE(GraphBuilder, "Layers");

	uint32 IndexBufferStart = 0;
	const auto& GraphicCaches = DeviceRGBSubsystem->GetGraphicsCaches();
	for (int32 LayerIndex = 0; LayerIndex < GraphicCaches.Num(); LayerIndex++)
	{
		FDeviceRGBMaterialCS::FParameters* Parameters = CreateParameters(GraphBuilder, View, Inputs, FrameResources);

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
			Parameters->NumItems = NumLeds;
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

	if (FrameResources.ColorBuffer)
	{
		GraphBuilder.QueueBufferExtraction(FrameResources.ColorBuffer, &ReadbackData, ERHIAccess::CPURead);
	}

	return Inputs.GetInput(EPostProcessMaterialInput::SceneColor);
}

void FDeviceRGBSceneViewExtension::EndFrame()
{
	SCOPE_CYCLE_COUNTER(STAT_EndFrame);

	if (!DeviceRGBSubsystem || !DeviceRGBSubsystem->IsEnabled() || NumLeds == 0)
	{
		return;
	}

	ENQUEUE_RENDER_COMMAND(FPerformEndOfFrameReadback)([this](FRHICommandListImmediate& RHICmdList)
	{
		SCOPE_CYCLE_COUNTER(STAT_Readback);

		if (ReadbackData && ReadbackData->GetStructuredBufferRHI() && ReadbackData->GetStructuredBufferRHI()->IsValid())
		{
			SCOPED_DRAW_EVENT(RHICmdList, DeviceRGB_DataReadback);

			TArray<FLinearColor> ColorsTemp;
			ColorsTemp.Init(FLinearColor::Black, NumLeds);

			const uint32 Size = static_cast<uint32>(ColorsTemp.Num()) * ColorsTemp.GetTypeSize();

			{
				SCOPE_CYCLE_COUNTER(STAT_ReadbackLock);

				void* Source = RHICmdList.LockStructuredBuffer(ReadbackData->GetStructuredBufferRHI(), 0, Size, RLM_ReadOnly);
				FMemory::Memcpy(ColorsTemp.GetData(), Source, Size);
				RHICmdList.UnlockStructuredBuffer(ReadbackData->GetStructuredBufferRHI());
			}
			
			AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, Colors = MoveTemp(ColorsTemp)]()
			{
				SCOPE_CYCLE_COUNTER(STAT_SetColors);

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
