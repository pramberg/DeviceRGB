// Copyright(c) 2021 Viktor Pramberg
#pragma once
#include "CoreMinimal.h"
#include "IDeviceRGB.h"

class FWootingDevice : public FDeviceRGB
{
public:
    FWootingDevice();
    
    virtual bool SetColors(const TArray<FColor> &InColors, bool bInFlushBuffers = true) override;
    virtual FVector2D GetPhysicalSize() const override;
    virtual EDeviceRGBType GetType() const override { return EDeviceRGBType::Keyboard; }

private:
	TArray<FWootingColor> Colors;
    FVector2D DeviceSize;
    EDeviceRGBType Type;
};