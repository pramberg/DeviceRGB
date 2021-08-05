// Copyright(c) 2021 Viktor Pramberg
#include "CorsairDevice.h"
#include <CUESDK.h>
#include <algorithm>

FCorsairDevice::FCorsairDevice(int32 InDeviceIndex) : DeviceIndex(InDeviceIndex)
{
	auto* Positions = CorsairGetLedPositionsByDeviceIndex(DeviceIndex);
	if (!Positions)
	{
		return;
	}

	NumLEDs = Positions->numberOfLed;
	Colors.Reserve(NumLEDs);
	LEDInfos.Reserve(NumLEDs);
	
	DeviceSize = GetDeviceSize(Positions);
	const float AspectRatio = FMath::Max(DeviceSize.X, DeviceSize.Y) / FMath::Min(DeviceSize.X, DeviceSize.Y);
	for (int32 i = 0; i < NumLEDs; i++)
	{
		const auto CurrentLED = Positions->pLedPosition[i];

		const FVector2D UV = [this, &CurrentLED, AspectRatio]() -> FVector2D
		{
			auto asdf = FVector2D
			{
				(float)(CurrentLED.left - CurrentLED.width * 0.5),
				(float)(CurrentLED.top - CurrentLED.height * 0.5)
			} / DeviceSize;
			asdf.X = FMath::Clamp(asdf.X, 0.0f, 1.0f);
			asdf.Y = FMath::Clamp(asdf.Y, 0.0f, 1.0f);
			return asdf;
		}();

		LEDInfos.Add(DeviceLEDInfo{ UV });
		Colors.Add({ CurrentLED.ledId, 0, 0, 0});
	}

	Type = EDeviceRGBType::Keyboard;
}

int32 FCorsairDevice::GetNumLEDs() const
{
	return NumLEDs;
}

bool FCorsairDevice::SetColors(const TArray<FColor>& InColors, bool bInFlushBuffers /*= true*/)
{
	IDevice::SetColors(InColors, bInFlushBuffers);

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
		FCorsairDeviceSDK::FlushBuffersImpl();
	}
	return bSuccessful;
}

TArray<DeviceLEDInfo> FCorsairDevice::GetLEDInfos() const
{
	return LEDInfos;
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

