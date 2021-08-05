// Copyright(c) 2021 Viktor Pramberg
#pragma once
#include "CoreMinimal.h"

class IDevice;

class IDeviceSDK
{
public:
	IDeviceSDK() = default;
	virtual ~IDeviceSDK() = default;

	/**
	 * If this returns true, it means that the SDK can be accessed as intended. If the function returns false, the object is invalid, and it will be destroyed.
	 */
	virtual bool IsValid() const { return true; }

	virtual int32 GetNumberOfDevices() const = 0;
	virtual void FlushBuffers() = 0;

	// Some SDKs can optimize by only flushing the colors after all color buffers have been set on all devices.
	// This function enables that by letting the user
	virtual bool SetColors(TFunctionRef<void(IDevice*, TArray<FColor>&)> InFunction);

	virtual void ForEachDevice(TFunctionRef<void(IDevice*)> InFunction) = 0;
};