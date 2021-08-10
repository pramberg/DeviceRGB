// Copyright(c) 2021 Viktor Pramberg
#pragma once
#include "CoreMinimal.h"
#include "IDevice.h"
#include "RazerDeviceSDK.h"
#include <array>

template<ERazerDeviceType DeviceType> int32 GetDeviceWidth() { return 0; }
template<> int32 GetDeviceWidth<ERazerDeviceType::Keyboard>() { return ChromaSDK::Keyboard::v2::MAX_COLUMN; }
template<> int32 GetDeviceWidth<ERazerDeviceType::Headset>() { return ChromaSDK::Headset::MAX_LEDS; }
template<> int32 GetDeviceWidth<ERazerDeviceType::Mousepad>() { return ChromaSDK::Mousepad::v2::MAX_LEDS; }
template<> int32 GetDeviceWidth<ERazerDeviceType::Mouse>() { return ChromaSDK::Mouse::MAX_COLUMN; }
template<> int32 GetDeviceWidth<ERazerDeviceType::Keypad>() { return ChromaSDK::Keypad::MAX_COLUMN; }
template<> int32 GetDeviceWidth<ERazerDeviceType::ChromaLink>() { return ChromaSDK::ChromaLink::MAX_LEDS; }

template<ERazerDeviceType DeviceType> int32 GetDeviceHeight() { return 0; }
template<> int32 GetDeviceHeight<ERazerDeviceType::Keyboard>() { return ChromaSDK::Keyboard::v2::MAX_ROW; }
template<> int32 GetDeviceHeight<ERazerDeviceType::Headset>() { return 1; }
template<> int32 GetDeviceHeight<ERazerDeviceType::Mousepad>() { return 1; }
template<> int32 GetDeviceHeight<ERazerDeviceType::Mouse>() { return ChromaSDK::Mouse::MAX_ROW; }
template<> int32 GetDeviceHeight<ERazerDeviceType::Keypad>() { return ChromaSDK::Keypad::MAX_ROW; }
template<> int32 GetDeviceHeight<ERazerDeviceType::ChromaLink>() { return 1; }

template<ERazerDeviceType DeviceType> constexpr int32 GetDeviceDimensions() { return 0; }
template<> constexpr int32 GetDeviceDimensions<ERazerDeviceType::Keyboard>() { return 2; }
template<> constexpr int32 GetDeviceDimensions<ERazerDeviceType::Headset>() { return 1; }
template<> constexpr int32 GetDeviceDimensions<ERazerDeviceType::Mousepad>() { return 1; }
template<> constexpr int32 GetDeviceDimensions<ERazerDeviceType::Mouse>() { return 2; }
template<> constexpr int32 GetDeviceDimensions<ERazerDeviceType::Keypad>() { return 2; }
template<> constexpr int32 GetDeviceDimensions<ERazerDeviceType::ChromaLink>() { return 1; }

template<ERazerDeviceType DeviceType> struct FCustomEffectContainer { ChromaSDK::CUSTOM_EFFECT_TYPE Effect{}; };
template<> struct FCustomEffectContainer<ERazerDeviceType::Keyboard> { ChromaSDK::Keyboard::v2::CUSTOM_EFFECT_TYPE Effect{}; };
template<> struct FCustomEffectContainer<ERazerDeviceType::Headset> { ChromaSDK::Headset::CUSTOM_EFFECT_TYPE Effect{}; };
template<> struct FCustomEffectContainer<ERazerDeviceType::Mousepad> { ChromaSDK::Mousepad::v2::CUSTOM_EFFECT_TYPE Effect{}; };
template<> struct FCustomEffectContainer<ERazerDeviceType::Mouse> { ChromaSDK::Mouse::CUSTOM_EFFECT_TYPE2 Effect{}; };
template<> struct FCustomEffectContainer<ERazerDeviceType::Keypad> { ChromaSDK::Keypad::CUSTOM_EFFECT_TYPE Effect{}; };
template<> struct FCustomEffectContainer<ERazerDeviceType::ChromaLink> { ChromaSDK::ChromaLink::CUSTOM_EFFECT_TYPE Effect{}; };

template<ERazerDeviceType DeviceType> FVector2D GetUV(const FIntPoint& InPosition, const FVector2D& InDeviceSize)
{
	return FVector2D(InPosition) / InDeviceSize;
}

// Because Razer's mousepads are 1D, we have to remap them to 2D manually.
template<> FVector2D GetUV<ERazerDeviceType::Mousepad>(const FIntPoint& InPosition, const FVector2D& InDeviceSize)
{
	constexpr float OneTenth = 1.0f / 10.0f;
	constexpr float OneFourteenth = 1.0f / 14.0f;
	static const std::array<FVector2D, 15> Positions
	{
		FVector2D(OneFourteenth * 13.0f, OneTenth * 1.0f),
		FVector2D(OneFourteenth * 13.0f, OneTenth * 3.0f),
		FVector2D(OneFourteenth * 13.0f, OneTenth * 5.0f),
		FVector2D(OneFourteenth * 13.0f, OneTenth * 7.0f),
		FVector2D(OneFourteenth * 13.0f, OneTenth * 9.0f),
		FVector2D(OneFourteenth * 11.0f, OneTenth * 9.0f),
		FVector2D(OneFourteenth * 9.0f, OneTenth * 9.0f),
		FVector2D(OneFourteenth * 7.0f, OneTenth * 9.0f),
		FVector2D(OneFourteenth * 5.0f, OneTenth * 9.0f),
		FVector2D(OneFourteenth * 3.0f, OneTenth * 9.0f),
		FVector2D(OneFourteenth * 1.0f, OneTenth * 9.0f),
		FVector2D(OneFourteenth * 1.0f, OneTenth * 7.0f),
		FVector2D(OneFourteenth * 1.0f, OneTenth * 5.0f),
		FVector2D(OneFourteenth * 1.0f, OneTenth * 3.0f),
		FVector2D(OneFourteenth * 1.0f, OneTenth * 1.0f)
	};

	return Positions[InPosition.X];
}

// This is a template so we can get the correct function ptr at compile time
template<ERazerDeviceType DeviceType>
class FRazerDevice : public IDevice
{
public:
	FRazerDevice(FRazerDeviceSDK* InSDK) : SDK(InSDK)
	{
		const int32 DeviceWidth = GetDeviceWidth<DeviceType>();
		const int32 DeviceHeight = GetDeviceHeight<DeviceType>();
		DeviceSize = FVector2D{ static_cast<float>(DeviceWidth), static_cast<float>(DeviceHeight) };

		FIntPoint Position{ EForceInit::ForceInitToZero };
		for (; Position.Y < DeviceHeight; Position.Y++)
		{
			for (; Position.X < DeviceWidth; Position.X++)
			{
				LEDInfos.Add({ GetUV<DeviceType>(Position, DeviceSize) });
				SetEffectColor(Position, FColor::Red);
			}
			Position.X = 0;
		}
	}

	virtual int32 GetNumLEDs() const override
	{
		return GetDeviceWidth<DeviceType>() * GetDeviceHeight<DeviceType>();
	}

	virtual bool SetColors(const TArray<FColor>& InColors, bool bInFlushBuffers = true) override
	{
		auto It = InColors.CreateConstIterator();
		FIntPoint Position{ EForceInit::ForceInitToZero };
		for (; Position.Y < GetDeviceHeight<DeviceType>(); Position.Y++)
		{
			for (; Position.X < GetDeviceWidth<DeviceType>(); Position.X++)
			{
				SetEffectColor(Position, *It);
				++It;
			}
			Position.X = 0;
		}

		if constexpr (DeviceType == ERazerDeviceType::Keyboard)
		{
			SDK->CreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_CUSTOM2, &EffectContainer.Effect, nullptr);
		}
		else if constexpr (DeviceType == ERazerDeviceType::Headset)
		{
			SDK->CreateHeadsetEffect(ChromaSDK::Headset::CHROMA_CUSTOM, &EffectContainer.Effect, nullptr);
		}
		else if constexpr (DeviceType == ERazerDeviceType::Mousepad)
		{
			SDK->CreateMousepadEffect(ChromaSDK::Mousepad::CHROMA_CUSTOM2, &EffectContainer.Effect, nullptr);
		}
		else if constexpr (DeviceType == ERazerDeviceType::Mouse)
		{
			SDK->CreateMouseEffect(ChromaSDK::Mouse::CHROMA_CUSTOM2, &EffectContainer.Effect, nullptr);
		}
		else if constexpr (DeviceType == ERazerDeviceType::Keypad)
		{
			SDK->CreateKeypadEffect(ChromaSDK::Keypad::CHROMA_CUSTOM, &EffectContainer.Effect, nullptr);
		}
		else if constexpr (DeviceType == ERazerDeviceType::ChromaLink)
		{
			SDK->CreateChromaLinkEffect(ChromaSDK::ChromaLink::CHROMA_CUSTOM, &EffectContainer.Effect, nullptr);
		}
		return true;
	}

	virtual TArray<FDeviceLEDInfo> GetLEDInfos() const override { return LEDInfos; }
	virtual FVector2D GetPhysicalSize() const override { return DeviceSize; }
	virtual EDeviceRGBType GetType() const override { return EDeviceRGBType::Keyboard; }

private:
	void SetEffectColor(const FIntPoint& InPos, const FColor& InColor)
	{
		if constexpr (GetDeviceDimensions<DeviceType>() == 1)
		{
			EffectContainer.Effect.Color[InPos.X] = FColorToRazerColor(InColor);
		}
		else if constexpr (GetDeviceDimensions<DeviceType>() == 2)
		{
			EffectContainer.Effect.Color[InPos.Y][InPos.X] = FColorToRazerColor(InColor);
		}
	}

	constexpr RZCOLOR FColorToRazerColor(const FColor& InColor)
	{
		return (InColor.R & 0xFF) | ((InColor.G & 0xFF) << 8) | ((InColor.B & 0xFF) << 16);
	}

	FColor ToFColor(const RZCOLOR InColor)
	{
		const uint8 Red = (InColor & 0xFF);
		const uint8 Green = ((InColor & 0xFF00) >> 8);
		const uint8 Blue = ((InColor & 0xFF0000) >> 16);
		const uint8 Alpha = ((InColor & 0xFF000000) >> 24);
		return FColor(Red, Green, Blue, Alpha);
	}

	TArray<FDeviceLEDInfo> LEDInfos;
	FCustomEffectContainer<DeviceType> EffectContainer;
	FVector2D DeviceSize;
	EDeviceRGBType Type;
	int32 DeviceIndex;
	int32 NumLEDs;

	FRazerDeviceSDK* SDK;
};