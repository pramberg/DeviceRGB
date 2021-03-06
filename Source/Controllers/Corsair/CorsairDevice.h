// Copyright(c) 2021 Viktor Pramberg
#pragma once
#include "CoreMinimal.h"
#include "IDeviceRGB.h"
#include <CUESDK.h>

class FCorsairDevice : public FDeviceRGB
{
public:
	FCorsairDevice(int32 InDeviceIndex);
	virtual bool SetColors(const TArray<FColor>& InColors, bool bInFlushBuffers = true) override;
	virtual FVector2D GetPhysicalSize() const override;
	virtual EDeviceRGBType GetType() const override;

private:
	FVector2D GetDeviceSize(const CorsairLedPositions* InPositions);

	TArray<CorsairLedColor> Colors;
	FVector2D DeviceSize;
	EDeviceRGBType Type;
	int32 DeviceIndex;
};