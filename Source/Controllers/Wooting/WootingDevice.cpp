// Copyright(c) 2021 Viktor Pramberg
#include "WootingDevice.h"
#include "WootingController.h"
#include "WootingKeyConverter.h"

#include <wooting-rgb-sdk.h>

FVector2D Frac(const FVector2D& Value)
{
	return { FMath::Frac(Value.X), FMath::Frac(Value.Y) };
}

FWootingDevice::FWootingDevice()
{
	constexpr int32 DeviceWidth = WOOTING_RGB_COLS;
	constexpr int32 DeviceHeight = WOOTING_RGB_ROWS;

	{
		const auto* Info = wooting_rgb_device_info();
		if (Info && Info->connected)
		{
			DeviceSize = FVector2D{ static_cast<float>(Info->max_columns), static_cast<float>(Info->max_rows) };
		}
		else
		{
			DeviceSize = FVector2D{ static_cast<float>(DeviceWidth), static_cast<float>(DeviceHeight) };
		}
	}

	const int32 NumLEDs = DeviceWidth * DeviceHeight;
	ReserveLeds(NumLEDs);

	FIntPoint Position{ EForceInit::ForceInitToZero };
	for (; Position.Y < DeviceHeight; Position.Y++)
	{
		for (; Position.X < DeviceWidth; Position.X++)
		{
			Colors.Add(FColor::Black);
			AddLed({ Frac(FVector2D(Position) / DeviceSize) }, { FWootingKeyConverter::ToFKey(Position) });
		}
		Position.X = 0;
	}
}

bool FWootingDevice::SetColors(const TArray<FColor>& InColors, bool bInFlushBuffers /*= true*/)
{
	auto It = InColors.CreateConstIterator();
	for (auto& TargetColor : Colors)
	{
		TargetColor = FWootingColor(*It);
		++It;
	}

	check(Colors.Num() * Colors.GetTypeSize() == WOOTING_RGB_COLS * WOOTING_RGB_ROWS * 3);

	const bool bSuccess = wooting_rgb_array_set_full(reinterpret_cast<uint8*>(Colors.GetData()));

	if (bSuccess && bInFlushBuffers)
	{
		FWootingController::FlushBuffersImpl();
	}

	return bSuccess;
}

FVector2D FWootingDevice::GetPhysicalSize() const
{
	return DeviceSize;
}