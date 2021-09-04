// Copyright(c) 2021 Viktor Pramberg
#include "TemplateDevice.h"
#include "TemplateController.h"
#include "TemplateKeyConverter.h"


FTemplateDevice::FTemplateDevice()
{
}

bool FTemplateDevice::SetColors(const TArray<FColor>& InColors, bool bInFlushBuffers /*= true*/)
{
	return true;
}

FVector2D FTemplateDevice::GetPhysicalSize() const
{
	return DeviceSize;
}

EDeviceRGBType FTemplateDevice::GetType() const
{
	return Type;
}