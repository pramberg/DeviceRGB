// Copyright(c) 2021 Viktor Pramberg
#pragma once
#include "SceneViewExtension.h"
#include "RHI.h"
#include "RHIResources.h"
#include <RendererInterface.h>

class UDeviceRGBSubsystem;
class UMaterialInterface;
class FRDGTexture;

struct FDeviceRGBFrameResources
{
	FRDGBufferSRVRef UV_SRV = nullptr;
	FRDGBufferSRVRef IndexBuffer_SRV = nullptr;
	FRDGBufferUAVRef Colors_UAV = nullptr;
	FRDGBufferRef ColorBuffer = nullptr;
};

/**
  * Used to efficiently render a potential material to texture after the scene has been rendered. This is effectively a post process material that has
  * access to all scene textures, but is not actually displayed on screen.
  */
class FDeviceRGBSceneViewExtension : public FSceneViewExtensionBase, public IPersistentViewUniformBufferExtension
{
public:
	FDeviceRGBSceneViewExtension(const FAutoRegister& AutoRegister, UDeviceRGBSubsystem* InDeviceRGBSubsystem);

	//~ Begin FSceneViewExtensionBase Interface
	virtual void SetupViewFamily(FSceneViewFamily& InViewFamily) override {};
	virtual void SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView) override {};
	virtual void BeginRenderViewFamily(FSceneViewFamily& InViewFamily) override {};
	virtual void PreRenderViewFamily_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneViewFamily& InViewFamily) override {};
	virtual void PreRenderView_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneView& InView) override {};

	virtual bool IsActiveThisFrameInContext(FSceneViewExtensionContext& Context) const override;
	virtual void SubscribeToPostProcessingPass(EPostProcessingPass Pass, FAfterPassCallbackDelegateArray& InOutPassCallbacks, bool bIsPassEnabled) override;
	//~ End FSceneViewExtensionBase Interface
	
	FScreenPassTexture RenderLayers(FRDGBuilder& GraphBuilder, const FSceneView& View, const FPostProcessMaterialInputs& Inputs);

	/** Performs the readback from the GPU after the graph builder has executed. */
	virtual void EndFrame() override;

	class FDeviceRGBMaterialParameters* CreateParameters(FRDGBuilder& GraphBuilder, const FSceneView& View, const FPostProcessMaterialInputs& Inputs, const FDeviceRGBFrameResources& FrameResources);

	FDeviceRGBFrameResources GatherFrameResources(FRDGBuilder& GraphBuilder);

private:
	UDeviceRGBSubsystem* DeviceRGBSubsystem;

	/** Buffer to read back from after the frame finishes */
	TRefCountPtr<class FRDGPooledBuffer> ReadbackData;

	/** Persistent buffer for the UVs */
	TRefCountPtr<class FRDGPooledBuffer> UVData;

	/** Persistent buffer for the UVs */
	TRefCountPtr<class FRDGPooledBuffer> IndexData;

	/** Persistent buffer for the colors */
	TRefCountPtr<class FRDGPooledBuffer> ColorData;

	mutable class FViewport* CurrentActiveViewport = nullptr;
};
