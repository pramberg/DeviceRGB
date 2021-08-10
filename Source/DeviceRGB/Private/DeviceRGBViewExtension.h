// Copyright(c) 2021 Viktor Pramberg
#pragma once
#include "SceneViewExtension.h"
#include "RHI.h"
#include "RHIResources.h"
#include <RendererInterface.h>

class UDeviceRGBSubsystem;
class UMaterialInterface;
class FRDGTexture;

/**
  * Used to efficiently render a potential material to texture after the scene has been rendered. This is effectively a post process material that has
  * access to all scene textures, but is not actually displayed on screen.
  */
class FDeviceRGBSceneViewExtension : public FSceneViewExtensionBase, public IPersistentViewUniformBufferExtension
{
public:
	FDeviceRGBSceneViewExtension(const FAutoRegister& AutoRegister, UDeviceRGBSubsystem* InEngineSubsystem);

	//~ Begin FSceneViewExtensionBase Interface
	virtual void PrePostProcessPass_RenderThread(FRDGBuilder& GraphBuilder, const FSceneView& View, const FPostProcessingInputs& Inputs) override;

	virtual void SetupViewFamily(FSceneViewFamily& InViewFamily) override;


	virtual void SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView) override;


	virtual void BeginRenderViewFamily(FSceneViewFamily& InViewFamily) override;


	virtual void PreRenderViewFamily_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneViewFamily& InViewFamily) override;


	virtual void PreRenderView_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneView& InView) override;


	virtual void PostRenderViewFamily_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneViewFamily& InViewFamily) override;


	virtual bool IsActiveThisFrameInContext(FSceneViewExtensionContext& Context) const override;


	virtual void PostRenderView_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneView& InView) override;


	virtual void EndFrame() override;

	//~ End FSceneViewExtensionBase Interface

private:
	UDeviceRGBSubsystem* EngineSubsystem;

	TRefCountPtr<class FRDGPooledBuffer> Data;

	TRefCountPtr<class FRDGPooledBuffer> UVData;
	TRefCountPtr<class FRDGPooledBuffer> ColorData;
};
