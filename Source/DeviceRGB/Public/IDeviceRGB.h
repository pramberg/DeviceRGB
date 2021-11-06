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
	Unknown UMETA(Hidden), // This can be used for SDKs where there is no distinction between device types.
};
ENUM_CLASS_FLAGS(EDeviceRGBType)

class IDeviceRGBController;

// Most important info
// #TODO: Remove this struct. UVs should just be stored as FVector2Ds directly. There is no need for this indirection.
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

/**
 * Represents whatever the idea of a device means for a IDeviceRGBController and its SDK.
 * 
 * In some SDKs, it can make sense to make each physical device its own instance
 * of your derived IDeviceRGB class (see FCorsairDevice). In other SDKs, it may make sense to only have
 * one IDeviceRGB that controls all physical devices (see FLogitechAddressableKeyboard and FLogitechGenericDevice).
 */
class IDeviceRGB
{
public:
	virtual ~IDeviceRGB() = default;

	/**
	 * Gets number of LEDs this device has.
	 * @returns	The number of LEDs.
	 */
	virtual int32 GetNumLEDs() const = 0;

	/**
	 * Sets the colors of all LEDs on this device.
	 *
	 * @param 	InColors	   	An array of all colors. Number of elements must be the same as the
	 * 							number of LEDs on this device.
	 * @param 	bInFlushBuffers	(Optional) True to flush buffers. Flushing manually (i.e. passing
	 * 							false and calling IDeviceRGBController::FlushBuffers()) can be more
	 * 							performant when updating multiple devices.
	 *
	 * @returns	True if it succeeds, false if it fails.
	 */
	virtual bool SetColors(const TArray<FColor>& InColors, bool bInFlushBuffers = true) = 0;

	/**
	 * Gets the LED infos.
	 */
	virtual TArray<FDeviceLEDInfo> GetLEDInfos() const = 0;

	/**
	 * Gets the size of the device.
	 * @returns	The physical size.
	 */
	virtual FVector2D GetPhysicalSize() const = 0;


	/**
	 * Gets the type
	 * @returns	The type.
	 */
	virtual EDeviceRGBType GetType() const = 0;

	/**
	 * Finds the indices to the LEDs that represent the keys in the array.
	 * @param 	InKeys	The keys to find.
	 * @returns	The indices.
	 */
	virtual TArray<int32> GetIndicesForKeys(const TArray<FKey>& InKeys) = 0;
};

/**
 * A base implementation of IDeviceRGB to reduce some of the boilerplate. Derive from this if
 * possible!
 */
class FDeviceRGB : public IDeviceRGB
{
public:
	virtual int32 GetNumLEDs() const override { return LEDInfos.Num(); }
	virtual TArray<FDeviceLEDInfo> GetLEDInfos() const override { return LEDInfos; }

	virtual TArray<int32> GetIndicesForKeys(const TArray<FKey>& InKeys) override
	{
		TArray<int32> Indices;
		for (const FKey& Key : InKeys)
		{
			const int32 Index = AdditionalInfos.IndexOfByPredicate([&Key](const auto& InAdditionalInfo) { return InAdditionalInfo.Key == Key; });
			if (Index != INDEX_NONE)
			{
				Indices.Add(Index);
			}
		}
		return Indices;
	}

	void ReserveLeds(int32 InNumLEDs)
	{
		LEDInfos.Reserve(InNumLEDs);
		AdditionalInfos.Reserve(InNumLEDs);
	}

	void AddLed(const FDeviceLEDInfo& InLedInfo, const FDeviceRGBAdditionalLEDInfo& InAdditionalInfo)
	{
		LEDInfos.Add(InLedInfo);
		AdditionalInfos.Add(InAdditionalInfo);
	}

protected:
	TArray<FDeviceLEDInfo> LEDInfos;
	TArray<FDeviceRGBAdditionalLEDInfo> AdditionalInfos;
};