// Copyright(c) 2021 Viktor Pramberg
#pragma once
#include "CoreMinimal.h"

class IDeviceRGB;

/**
 * A controller is responsible for giving the subsystem access to a manufacturer's SDK, and its devices.
 * Typically, it would load and unload the SDK's dll and create devices.
 */
class IDeviceRGBController
{
public:
	IDeviceRGBController() = default;
	virtual ~IDeviceRGBController() = default;

	/** Gets the number of devices */
	virtual int32 GetNumberOfDevices() const = 0;

	/**
	 * This will commit the changes from SetColors() and actually update the colors on the device(s).
	 * Some SDKs don't need to do this, and might flush internally every time SetColors() is called.
	 */
	virtual void FlushBuffers() {}

	/**
	 * Will call InFunction for each device managed by this controller.
	 * @param [in]	InFunction	The function to call.
	 */
	virtual void ForEachDevice(TFunctionRef<void(IDeviceRGB*)> InFunction) = 0;


	/**
	 * Enables or disables this controller. In cases where the SDK doesn't have the concept of
	 * releasing control temporarily, the color of all LEDs should be set to black instead when
	 * disabling the controller.
	 *
	 * @param 	bEnabled	True to enable, false to disable.
	 */
	virtual void SetEnabled(bool bEnabled) = 0;
};

/**
 * A base implementation of IDeviceRGBController to reduce some of the boilerplate. Derive from
 * this if possible!
 */
class FDeviceRGBController : public IDeviceRGBController
{
public:
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