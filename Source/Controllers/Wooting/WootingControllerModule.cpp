// Copyright(c) 2021 Viktor Pramberg
#include "Modules/ModuleManager.h"
#include "WootingController.h"
#include "DeviceRGB.h"

class FWootingControllerModule : public IModuleInterface
{
public:
	virtual void StartupModule() override
	{
		FModuleManager::GetModuleChecked<FDeviceRGBModule>("DeviceRGB").RegisterController(FWootingController::Construct());
	}
};

IMPLEMENT_MODULE(FWootingControllerModule, WootingController)