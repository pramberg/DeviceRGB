#include "LogitechGenericDevice.h"
#include "LogitechController.h"

#include "LogitechLEDLib.h"

FLogitechGenericDevice::FLogitechGenericDevice()
{
	LEDInfos.Add({ FVector2D(0.5f) });
}

bool FLogitechGenericDevice::SetColors(const TArray<FColor>& InColors, bool bInFlushBuffers /*= true*/)
{
	Color = FLogitechColor(InColors[0]);

	LogiLedSetTargetDevice(LOGI_DEVICETYPE_MONOCHROME | LOGI_DEVICETYPE_RGB);
	return LogiLedSetLighting(Color.R_Percent(), Color.G_Percent(), Color.B_Percent());
}

TArray<int32> FLogitechGenericDevice::GetIndicesForKeys(const TArray<FKey>& InKeys)
{
	return TArray<int32>();
}
