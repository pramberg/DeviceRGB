// Copyright(c) 2021 Viktor Pramberg
#include "CorsairDevice.h"
#include "CorsairController.h"
#include <CUESDK.h>
#include <algorithm>
#include "CorsairKeyConverter.h"


FCorsairDevice::FCorsairDevice(int32 InDeviceIndex) : DeviceIndex(InDeviceIndex)
{
	auto* Positions = CorsairGetLedPositionsByDeviceIndex(DeviceIndex);
	if (!Positions)
	{
		return;
	}

	const int32 NumLEDs = Positions->numberOfLed;
	Colors.Reserve(NumLEDs);
	ReserveLeds(NumLEDs);
	
	DeviceSize = GetDeviceSize(Positions);
	const float AspectRatio = FMath::Max(DeviceSize.X, DeviceSize.Y) / FMath::Min(DeviceSize.X, DeviceSize.Y);
	for (int32 i = 0; i < NumLEDs; i++)
	{
		const auto CurrentLED = Positions->pLedPosition[i];

		const FVector2D UV = [this, &CurrentLED, AspectRatio]() -> FVector2D
		{
			auto LedCenter = FVector2D
			{
				(float)(CurrentLED.left - CurrentLED.width * 0.5),
				(float)(CurrentLED.top - CurrentLED.height * 0.5)
			} / DeviceSize;
			LedCenter.X = FMath::Clamp(LedCenter.X, 0.0f, 1.0f);
			LedCenter.Y = FMath::Clamp(LedCenter.Y, 0.0f, 1.0f);
			return LedCenter;
		}();

		Colors.Add({ CurrentLED.ledId, 0, 0, 0});
		AddLed({ UV }, { FCorsairKeyConverter::ToFKey(CurrentLED.ledId) });
	}

	auto* DeviceInfo = CorsairGetDeviceInfo(DeviceIndex);
	switch (DeviceInfo->type)
	{
	case CDT_Unknown:
		Type = EDeviceRGBType::Other;
		break;
	case CDT_Mouse:
		Type = EDeviceRGBType::Mouse;
		break;
	case CDT_Keyboard:
		Type = EDeviceRGBType::Keyboard;
		break;
	case CDT_Headset:
		Type = EDeviceRGBType::Headset;
		break;
	case CDT_MouseMat:
		Type = EDeviceRGBType::Mousepad;
		break;
	case CDT_HeadsetStand:
		Type = EDeviceRGBType::Other;
		break;
	case CDT_CommanderPro:
		Type = EDeviceRGBType::Other;
		break;
	case CDT_LightingNodePro:
		Type = EDeviceRGBType::Other;
		break;
	case CDT_MemoryModule:
		Type = EDeviceRGBType::Other;
		break;
	case CDT_Cooler:
		Type = EDeviceRGBType::Other;
		break;
	case CDT_Motherboard:
		Type = EDeviceRGBType::Other;
		break;
	case CDT_GraphicsCard:
		Type = EDeviceRGBType::Other;
		break;
	}
}

bool FCorsairDevice::SetColors(const TArray<FColor>& InColors, bool bInFlushBuffers /*= true*/)
{
	auto It = InColors.CreateConstIterator();
	for (auto& CorsairColor : Colors)
	{
		const FColor& Color = *It;
		CorsairColor.r = Color.R;
		CorsairColor.g = Color.G;
		CorsairColor.b = Color.B;
		++It;
	}

	bool bSuccessful = CorsairSetLedsColorsBufferByDeviceIndex(DeviceIndex, Colors.Num(), Colors.GetData());

	if (bInFlushBuffers && bSuccessful)
	{
		FCorsairController::FlushBuffersImpl();
	}
	return bSuccessful;
}

FVector2D FCorsairDevice::GetPhysicalSize() const
{
	return DeviceSize;
}

EDeviceRGBType FCorsairDevice::GetType() const
{
	return Type;
}

FVector2D FCorsairDevice::GetDeviceSize(const CorsairLedPositions* InPositions)
{
	const auto minmaxWidth = std::minmax_element(
		InPositions->pLedPosition,
		InPositions->pLedPosition + InPositions->numberOfLed,
		[](const CorsairLedPosition& clp1, const CorsairLedPosition& clp2)
	{
		return clp1.left < clp2.left;
	}
	);

	const auto minmaxHeight = std::minmax_element(
		InPositions->pLedPosition,
		InPositions->pLedPosition + InPositions->numberOfLed,
		[](const CorsairLedPosition& clp1, const CorsairLedPosition& clp2)
	{
		return clp1.top < clp2.top;
	}
	);

	return {
		static_cast<float>(minmaxWidth.second->left + minmaxWidth.second->width - minmaxWidth.first->left),
		static_cast<float>(minmaxHeight.second->top + minmaxHeight.second->height - minmaxHeight.first->top)
	};
}