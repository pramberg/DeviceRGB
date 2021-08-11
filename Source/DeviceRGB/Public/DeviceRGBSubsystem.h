// Copyright(c) 2021 Viktor Pramberg
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "DeviceRGBViewExtension.h"
#include <Tickable.h>
#include "IDeviceSDK.h"
#include "IDevice.h"
#include "DeviceRGBSubsystem.generated.h"

class UTexture2D;
class UMaterialInterface;

/**
 * A centralized point of interaction for all of DeviceRGB's features.
 */
UCLASS(meta = (DisplayName = "DeviceRGB Subsystem"))
class DEVICERGB_API UDeviceRGBSubsystem : public UEngineSubsystem
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

	// Only Windows is supported.
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	const TArray<FDeviceLEDInfo>& GetCachedInfos() const { return CachedLEDInfos; }

	void SetColors(TFunctionRef<void(IDevice*, TArray<FColor>&)> InFunction);

private:
	TVariant<UTexture2D*, UMaterialInterface*> CurrentGraphic;

	TSharedPtr<FDeviceRGBSceneViewExtension, ESPMode::ThreadSafe> ViewExtension;

	TArray<TUniquePtr<IDeviceSDK>> SupportedSDKs;
	// Contains all connected devices' infos. 
	TArray<FDeviceLEDInfo> CachedLEDInfos;
};
