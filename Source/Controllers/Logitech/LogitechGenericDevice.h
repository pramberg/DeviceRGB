// Copyright(c) 2021 Viktor Pramberg
#pragma once
#include "CoreMinimal.h"
#include "LogitechDeviceBase.h"
#include "LogitechKeyConverter.h"

/** A generic Logitech device that handles all other devices that FLogitechAddressableKeyboard doesn't handle. */
class FLogitechGenericDevice : public FLogitechDeviceBase
{
public:
	FLogitechGenericDevice();

	virtual int32 GetNumLEDs() const override { return 1; }
	virtual bool SetColors(const TArray<FColor>& InColors, bool bInFlushBuffers = true) override;
	virtual TArray<FDeviceLEDInfo> GetLEDInfos() const override { return LEDInfos; }
	virtual FVector2D GetPhysicalSize() const override { return DeviceSize; }
	virtual EDeviceRGBType GetType() const override { return EDeviceRGBType::Unknown; }
	virtual TArray<int32> GetIndicesForKeys(const TArray<FKey>& InKeys) override;

private:
	TArray<FDeviceLEDInfo> LEDInfos;
	FLogitechColor Color = FLogitechColor(FColor::Black);

	FVector2D DeviceSize;
};