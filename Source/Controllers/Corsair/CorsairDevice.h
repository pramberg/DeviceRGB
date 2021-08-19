// Copyright(c) 2021 Viktor Pramberg
#pragma once
#include "CoreMinimal.h"
#include "IDeviceRGB.h"
#include <CUESDK.h>

class FCorsairDevice : public IDeviceRGB
{
public:
	FCorsairDevice(int32 InDeviceIndex);
	virtual int32 GetNumLEDs() const override;
	virtual bool SetColors(const TArray<FColor>& InColors, bool bInFlushBuffers = true) override;
	virtual TArray<FDeviceLEDInfo> GetLEDInfos() const override;
	virtual FVector2D GetPhysicalSize() const override;
	virtual EDeviceRGBType GetType() const override;

	virtual TArray<int32> GetIndicesForKeys(const TArray<FKey>& InKeys) override;
private:
	FVector2D GetDeviceSize(const CorsairLedPositions* InPositions);

	TArray<FDeviceLEDInfo> LEDInfos;
	TArray<CorsairLedColor> Colors;
	TArray<FDeviceRGBAdditionalLEDInfo> AdditionalInfos;
	FVector2D DeviceSize;
	EDeviceRGBType Type;
	int32 DeviceIndex;
	int32 NumLEDs;
};