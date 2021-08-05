// Copyright(c) 2021 Viktor Pramberg
#pragma once
#include "CoreMinimal.h"

enum class EDeviceRGBType : uint8
{
	Keyboard,
	Keypad,
	Mouse,
	Mousepad,
	Headset,
};

class IDeviceSDK;

// Most important info
struct DeviceLEDInfo
{
	// Normalized location
	FVector2D UV;
};

struct AdditionalDeviceLEDInfo
{
public:
	FKey Key;
	FVector2D Size;
	FVector2D Location;
};

class IDevice
{
public:
	virtual ~IDevice() = default;
	virtual int32 GetNumLEDs() const = 0;
	virtual bool SetColors(const TArray<FColor>& InColors, bool bInFlushBuffers = true);

	/**
	 * Gets the LED infos.
	 */
	virtual TArray<DeviceLEDInfo> GetLEDInfos() const = 0;

	/**
	 * Gets the size of the device. This
	 *
	 * @returns	The physical size.
	 */
	virtual FVector2D GetPhysicalSize() const = 0;
	virtual EDeviceRGBType GetType() const = 0;
};