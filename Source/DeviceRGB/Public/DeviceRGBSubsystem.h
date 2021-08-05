// Copyright(c) 2021 Viktor Pramberg
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "DeviceRGBViewExtension.h"
#include <Tickable.h>
#include "IDeviceSDK.h"
#include "DeviceRGBSubsystem.generated.h"

class UTexture2D;
class UMaterialInterface;

/**
 * A centralized point of interaction for all of DeviceRGB's features.
 */
UCLASS(meta = (DisplayName = "DeviceRGB Subsystem"))
class DEVICERGB_API UDeviceRGBSubsystem : public UEngineSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "DeviceRGB")
	void SetTexture(UTexture2D* InTexture);

	UFUNCTION(BlueprintCallable, Category = "DeviceRGB")
	void SetMaterial(UMaterialInterface* InMaterial);

	decltype(auto) GetCurrentGraphic() const { return (CurrentGraphic); }


	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;


	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override;


	virtual UWorld* GetTickableGameObjectWorld() const override;


	virtual bool IsTickableInEditor() const override;

	// Only Windows is supported.
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

private:
	TVariant<UTexture2D*, UMaterialInterface*> CurrentGraphic;

	TSharedPtr<FDeviceRGBSceneViewExtension, ESPMode::ThreadSafe> ViewExtension;

	UPROPERTY()
	class UTextureRenderTarget2D* RenderTarget = nullptr;

	UPROPERTY()
	class USceneCaptureComponent2D* Capture = nullptr;

	TArray<TUniquePtr<IDeviceSDK>> SupportedSDKs;
};
