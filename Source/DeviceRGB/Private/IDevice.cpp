// Copyright(c) 2021 Viktor Pramberg
#pragma once
#include "IDevice.h"
#include "IDeviceSDK.h"

bool IDevice::SetColors(const TArray<FColor>& InColors, bool bInFlushBuffers)
{
	ensure(InColors.Num() == GetNumLEDs());
	return true;
}
