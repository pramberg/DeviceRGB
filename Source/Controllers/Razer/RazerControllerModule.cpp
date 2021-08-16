// Copyright(c) 2021 Viktor Pramberg
#include "Modules/ModuleManager.h"
#include "RazerController.h"
#include "DeviceRGB.h"

class FRazerControllerModule : public IModuleInterface
{
public:
	virtual void StartupModule() override
	{
		FModuleManager::GetModuleChecked<FDeviceRGBModule>("DeviceRGB").RegisterController(FRazerController::Construct());
	}

	virtual void ShutdownModule() override
	{

	}
};

IMPLEMENT_MODULE(FRazerControllerModule, RazerController)