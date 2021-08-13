// Copyright(c) 2021 Viktor Pramberg
#pragma once
#include "CoreMinimal.h"

class IDeviceRGB;

class IDeviceRGBController
{
public:
	IDeviceRGBController() = default;
	virtual ~IDeviceRGBController() = default;

	/**
	 * If this returns true, it means that the controller can be accessed as intended. If the function returns false, the object is invalid, and it will be destroyed.
	 */
	virtual bool IsValid() const { return true; }

	virtual int32 GetNumberOfDevices() const = 0;
	virtual void FlushBuffers() = 0;
	virtual void ForEachDevice(TFunctionRef<void(IDeviceRGB*)> InFunction) = 0;
};