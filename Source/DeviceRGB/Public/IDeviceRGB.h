// Copyright(c) 2021 Viktor Pramberg
#pragma once
#include "CoreMinimal.h"
#include "IDeviceRGB.generated.h"

UENUM(BlueprintType, meta = (Bitflags))
enum class EDeviceRGBType : uint8
{
	Keyboard,
	Keypad,
	Mouse,
	Mousepad,
	Headset,
	Other,
};
ENUM_CLASS_FLAGS(EDeviceRGBType)

class IDeviceRGBController;

// Most important info
struct FDeviceLEDInfo
{
	// Normalized location
	FVector2D UV;
};

struct FDeviceRGBAdditionalLEDInfo
{
public:
	FKey Key;
	FVector2D Size;
	FVector2D Location;
};

class IDeviceRGB
{
public:
	virtual ~IDeviceRGB() = default;
	virtual int32 GetNumLEDs() const = 0;
	virtual bool SetColors(const TArray<FColor>& InColors, bool bInFlushBuffers = true) = 0;

	/**
	 * Gets the LED infos.
	 */
	virtual TArray<FDeviceLEDInfo> GetLEDInfos() const = 0;

	/**
	 * Gets the size of the device. This
	 *
	 * @returns	The physical size.
	 */
	virtual FVector2D GetPhysicalSize() const = 0;
	virtual EDeviceRGBType GetType() const = 0;

	virtual TArray<int32> GetIndicesForKeys(const TArray<FKey>& InKeys) = 0;
};