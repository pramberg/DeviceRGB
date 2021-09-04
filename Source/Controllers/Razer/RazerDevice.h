// Copyright(c) 2021 Viktor Pramberg
#pragma once
#include "CoreMinimal.h"
#include "RazerController.h"
#include "IDeviceRGB.h"
#include <array>
#include "RazerKeyConverter.h"

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

class FRazerDeviceBase : public FDeviceRGB
{
private:
	virtual void SetStaticColor(const FColor& InColor) = 0;
	friend FRazerController;
};

// This is a template so we can get the correct function ptr at compile time
template<ERazerDeviceType DeviceType>
class FRazerDevice : public FRazerDeviceBase
{
public:
	FRazerDevice(FRazerController* InController) : Controller(InController)
	{
		const int32 DeviceWidth = GetDeviceWidth<DeviceType>();
		const int32 DeviceHeight = GetDeviceHeight<DeviceType>();
		DeviceSize = FVector2D{ static_cast<float>(DeviceWidth), static_cast<float>(DeviceHeight) };

		const int32 NumLEDs = DeviceWidth * DeviceHeight;
		ReserveLeds(NumLEDs);

		FIntPoint Position{ EForceInit::ForceInitToZero };
		for (; Position.Y < DeviceHeight; Position.Y++)
		{
			for (; Position.X < DeviceWidth; Position.X++)
			{
				SetEffectColor(Position, FColor::Red);
				AddLed({ GetUV<DeviceType>(Position, DeviceSize) }, { FRazerKeyConverter::ToFKey<DeviceType>(Position) });
			}
			Position.X = 0;
		}

		Type = FRazerController::ToDeviceRGBType(DeviceType);
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

		RecreateEffects();
		return true;
	}

	void RecreateEffects()
	{
		if constexpr (DeviceType == ERazerDeviceType::Keyboard)
		{
			Controller->CreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_CUSTOM2, &EffectContainer.Effect, nullptr);
		}
		else if constexpr (DeviceType == ERazerDeviceType::Headset)
		{
			Controller->CreateHeadsetEffect(ChromaSDK::Headset::CHROMA_CUSTOM, &EffectContainer.Effect, nullptr);
		}
		else if constexpr (DeviceType == ERazerDeviceType::Mousepad)
		{
			Controller->CreateMousepadEffect(ChromaSDK::Mousepad::CHROMA_CUSTOM2, &EffectContainer.Effect, nullptr);
		}
		else if constexpr (DeviceType == ERazerDeviceType::Mouse)
		{
			Controller->CreateMouseEffect(ChromaSDK::Mouse::CHROMA_CUSTOM2, &EffectContainer.Effect, nullptr);
		}
		else if constexpr (DeviceType == ERazerDeviceType::Keypad)
		{
			Controller->CreateKeypadEffect(ChromaSDK::Keypad::CHROMA_CUSTOM, &EffectContainer.Effect, nullptr);
		}
		else if constexpr (DeviceType == ERazerDeviceType::ChromaLink)
		{
			Controller->CreateChromaLinkEffect(ChromaSDK::ChromaLink::CHROMA_CUSTOM, &EffectContainer.Effect, nullptr);
		}
	}

	virtual FVector2D GetPhysicalSize() const override { return DeviceSize; }
	virtual EDeviceRGBType GetType() const override { return Type; }

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

	virtual void SetStaticColor(const FColor& InColor) override
	{
		FIntPoint Position{ EForceInit::ForceInitToZero };
		for (; Position.Y < GetDeviceHeight<DeviceType>(); Position.Y++)
		{
			for (; Position.X < GetDeviceWidth<DeviceType>(); Position.X++)
			{
				SetEffectColor(Position, InColor);
			}
			Position.X = 0;
		}
		RecreateEffects();
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

	FCustomEffectContainer<DeviceType> EffectContainer;

	FVector2D DeviceSize;
	EDeviceRGBType Type;
	int32 DeviceIndex;

	FRazerController* Controller;
};