// Copyright(c) 2021 Viktor Pramberg
#pragma once
#include "CoreMinimal.h"
#include "IDeviceRGB.h"

class FTemplateDevice : public FDeviceRGB
{
public:
    FTemplateDevice();
    
    virtual bool SetColors(const TArray<FColor> &InColors, bool bInFlushBuffers = true) override;
    virtual FVector2D GetPhysicalSize() const override;
    virtual EDeviceRGBType GetType() const override;

private:
    FVector2D DeviceSize;
    EDeviceRGBType Type;
};