// Copyright(c) 2021 Viktor Pramberg
#include "DeviceRGBViewExtension.h"
#include "DeviceRGBSubsystem.h"
//#include <PostProcess/PostProcessMaterial.h>
//#include <ScreenPass.h>
//#include <PostProcess/PostProcessing.h>
//#include <SceneRenderTargetParameters.h>

FDeviceRGBSceneViewExtension::FDeviceRGBSceneViewExtension(const FAutoRegister& AutoRegister, UDeviceRGBSubsystem* InEngineSubsystem) : FSceneViewExtensionBase(AutoRegister), EngineSubsystem(InEngineSubsystem)
{

}

void FDeviceRGBSceneViewExtension::PrePostProcessPass_RenderThread(FRDGBuilder& GraphBuilder, const FSceneView& View, const FPostProcessingInputs& Inputs)
{
	checkSlow(View.bIsViewInfo);
	//FScreenPassTexture SceneColor((*Inputs.SceneTextures)->SceneColorTexture);

	//const auto GetPostProcessMaterialInputs = [&](FScreenPassTexture InSceneColor)
	//{
	//	FPostProcessMaterialInputs PostProcessMaterialInputs;

	//	//PostProcessMaterialInputs.SetInput(EPostProcessMaterialInput::SceneColor, InSceneColor);
	//	/*PostProcessMaterialInputs.SetInput(EPostProcessMaterialInput::SeparateTranslucency, SeparateTranslucency);
	//	PostProcessMaterialInputs.SetInput(EPostProcessMaterialInput::Velocity, Velocity);*/
	//	//PostProcessMaterialInputs.SceneTextures = GetSceneTextureShaderParameters(Inputs.SceneTextures);
	//	//PostProcessMaterialInputs.CustomDepthTexture = CustomDepth.Texture;

	//	return PostProcessMaterialInputs;
	//};

	//AddPostProcessMaterialPass(GraphBuilder, static_cast<const FViewInfo&>(View), GetPostProcessMaterialInputs(SceneColor), EngineSubsystem->GetCurrentGraphic().Get<UMaterialInterface*>());
}

bool FDeviceRGBSceneViewExtension::IsActiveThisFrame(class FViewport* InViewport) const
{
	return EngineSubsystem->GetCurrentGraphic().IsType<UMaterialInterface*>();
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
