// Copyright(c) 2021 Viktor Pramberg
#pragma once
#include "CoreMinimal.h"
#include "LogitechDeviceBase.h"

/** A representation of a keyboard with individually addressable LEDs. Note that not all Logitech keyboards can be represented with this class, which is why FLogitechDeviceSingleZone also exists. */
class FLogitechAddressableKeyboard : public FLogitechDeviceBase
{
public:
	FLogitechAddressableKeyboard();

	virtual int32 GetNumLEDs() const override;
	virtual bool SetColors(const TArray<FColor>& InColors, bool bInFlushBuffers = true) override;
	virtual TArray<FDeviceLEDInfo> GetLEDInfos() const override { return LEDInfos; }
	virtual FVector2D GetPhysicalSize() const override { return DeviceSize; }
	virtual EDeviceRGBType GetType() const override { return EDeviceRGBType::Keyboard; } // Per-key addressable devices can only be keyboards in the Logitech SDK
	virtual TArray<int32> GetIndicesForKeys(const TArray<FKey>& InKeys) override;

private:
	TArray<FDeviceLEDInfo> LEDInfos;
	TArray<struct FLogitechColor> Colors;
	TArray<FDeviceRGBAdditionalLEDInfo> AdditionalInfos;

	FVector2D DeviceSize;
};