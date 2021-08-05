// Copyright(c) 2021 Viktor Pramberg
#pragma once
#include "SceneViewExtension.h"
#include "RHI.h"
#include "RHIResources.h"

class UDeviceRGBSubsystem;
class UMaterialInterface;
class FRDGTexture;

/**
  * Used to efficiently render a potential material to texture after the scene has been rendered. This is effectively a post process material that has
  * access to all scene textures, but is not actually displayed on screen.
  */
class FDeviceRGBSceneViewExtension : public FSceneViewExtensionBase
{
public:
	FDeviceRGBSceneViewExtension(const FAutoRegister& AutoRegister, UDeviceRGBSubsystem* InEngineSubsystem);

	//~ Begin FSceneViewExtensionBase Interface
	virtual void PrePostProcessPass_RenderThread(FRDGBuilder& GraphBuilder, const FSceneView& View, const FPostProcessingInputs& Inputs) override;
	virtual bool IsActiveThisFrame(class FViewport* InViewport) const override;

	virtual void SetupViewFamily(FSceneViewFamily& InViewFamily) override;


	virtual void SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView) override;


	virtual void BeginRenderViewFamily(FSceneViewFamily& InViewFamily) override;


	virtual void PreRenderViewFamily_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneViewFamily& InViewFamily) override;


	virtual void PreRenderView_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneView& InView) override;

	//~ End FSceneViewExtensionBase Interface

private:
	UDeviceRGBSubsystem* EngineSubsystem;
};
