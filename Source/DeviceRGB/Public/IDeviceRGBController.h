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
	virtual void SetEnabled(bool bEnabled) = 0;

	virtual void* GetDLLHandle() const = 0;
};

class FDeviceRGBController : public IDeviceRGBController
{
public:
	virtual void* GetDLLHandle() const override { return nullptr; }

	virtual int32 GetNumberOfDevices() const override { return Devices.Num(); }
	virtual void ForEachDevice(TFunctionRef<void(IDeviceRGB*)> InFunction) override
	{
		for (auto& Device : Devices)
		{
			InFunction(Device.Get());
		}
	}

	template<typename T, typename... TArgs>
	void AddDevice(TArgs&&... InArgs)
	{
		Devices.Add(MakeUnique<T>(InArgs...));
	}

protected:
	TArray<TUniquePtr<IDeviceRGB>> Devices;
};