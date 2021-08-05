// Copyright(c) 2021 Viktor Pramberg
#pragma once
#include "IDeviceSDK.h"
#include "IDevice.h"

bool IDeviceSDK::SetColors(TFunctionRef<void(IDevice*, TArray<FColor>&)> InFunction)
{
	TArray<FColor> Colors;
	bool bSuccessful = true;
	ForEachDevice([&](IDevice* InDevice)
	{
		Colors.Empty(InDevice->GetNumLEDs());
		InFunction(InDevice, Colors);
		bSuccessful &= InDevice->SetColors(Colors, false);
	});
	FlushBuffers();
	return bSuccessful;
}