#include "LogitechAddressableKeyboard.h"
#include "LogitechKeyConverter.h"
#include "LogitechController.h"

#include "LogitechLEDLib.h"

FLogitechAddressableKeyboard::FLogitechAddressableKeyboard()
{
	const int32 DeviceWidth = LOGI_LED_BITMAP_WIDTH;
	const int32 DeviceHeight = LOGI_LED_BITMAP_HEIGHT;
	DeviceSize = FVector2D{ static_cast<float>(DeviceWidth), static_cast<float>(DeviceHeight) };

	const FLogitechColor DefaultColor = FLogitechColor(FColor::Black);
	for (int32 Y = 0; Y < DeviceHeight; Y++)
	{
		for (int32 X = 0; X < DeviceWidth; X++)
		{
			const int32 Index = Y * DeviceHeight + X;
			LEDInfos.Add({ FVector2D(X, Y) / DeviceSize });
			Colors.Add(DefaultColor);
			AdditionalInfos.Add({ FLogitechKeyConverter::ToFKey(X, Y) });
		}
	}
}

int32 FLogitechAddressableKeyboard::GetNumLEDs() const
{
	return LOGI_LED_BITMAP_WIDTH * LOGI_LED_BITMAP_HEIGHT;
}

bool FLogitechAddressableKeyboard::SetColors(const TArray<FColor>& InColors, bool bInFlushBuffers /*= true*/)
{
	auto It = InColors.CreateConstIterator();
	for (auto& Color : Colors)
	{
		Color = FLogitechColor(*It);
	}

	LogiLedSetTargetDevice(LOGI_DEVICETYPE_PERKEY_RGB);
	return LogiLedSetLightingFromBitmap(reinterpret_cast<uint8*>(Colors.GetData()));
}

TArray<int32> FLogitechAddressableKeyboard::GetIndicesForKeys(const TArray<FKey>& InKeys)
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
