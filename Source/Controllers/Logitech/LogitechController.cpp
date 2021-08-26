// Copyright(c) 2021 Viktor Pramberg
#include "LogitechController.h"
#include "DeviceRGBSettings.h"

#include "LogitechAddressableKeyboard.h"
#include "LogitechGenericDevice.h"

#include <GeneralProjectSettings.h>

FLogitechController::FLogitechController()
{
	
}

FLogitechController::~FLogitechController()
{
	LogiLedShutdown();
}

TUniquePtr<FLogitechController> FLogitechController::Construct()
{
	const auto* DeviceRGBSettings = GetDefault<UDeviceRGBSettings>();

	if (!LogiLedInitWithName(TCHAR_TO_ANSI(*DeviceRGBSettings->GetProjectName())))
	{
		return nullptr;
	}

	auto Controller = TUniquePtr<FLogitechController>(new FLogitechController());

	Controller->Devices.Add(MakeUnique<FLogitechAddressableKeyboard>());
	Controller->Devices.Add(MakeUnique<FLogitechGenericDevice>());

	return MoveTemp(Controller);
}

int32 FLogitechController::GetNumberOfDevices() const
{
	return Devices.Num();
}

void FLogitechController::ForEachDevice(TFunctionRef<void(IDeviceRGB*)> InFunction)
{
	for (auto& Device : Devices)
	{
		InFunction(Device.Get());
	}
}

void FLogitechController::SetEnabled(bool bEnabled)
{
	if (bEnabled)
	{
		return;
	}

	LogiLedSetTargetDevice(LOGI_DEVICETYPE_ALL);
	LogiLedSetLighting(0, 0, 0);
}