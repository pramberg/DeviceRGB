// Copyright(c) 2021 Viktor Pramberg
#pragma once
#include "CoreMinimal.h"

class IDeviceRGB;

class IDeviceRGBController
{
public:
	IDeviceRGBController() = default;
	virtual ~IDeviceRGBController() = default;

	virtual int32 GetNumberOfDevices() const = 0;
	virtual void FlushBuffers() = 0;
	virtual void ForEachDevice(TFunctionRef<void(IDeviceRGB*)> InFunction) = 0;

	virtual void* GetDLLHandle() const = 0;
};