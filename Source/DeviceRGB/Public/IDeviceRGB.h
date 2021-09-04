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