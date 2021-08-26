// Copyright(c) 2021 Viktor Pramberg
#include "Modules/ModuleManager.h"
#include "LogitechController.h"
#include "DeviceRGB.h"

class FLogitechControllerModule : public IModuleInterface
{
public:
	virtual void StartupModule() override
	{
		FModuleManager::GetModuleChecked<FDeviceRGBModule>("DeviceRGB").RegisterController(FLogitechController::Construct());
	}
};

IMPLEMENT_MODULE(FLogitechControllerModule, LogitechController)