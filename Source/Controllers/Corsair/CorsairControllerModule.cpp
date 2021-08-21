// Copyright(c) 2021 Viktor Pramberg
#include "Modules/ModuleManager.h"
#include "CorsairController.h"
#include "DeviceRGB.h"

class FCorsairControllerModule : public IModuleInterface
{
public:
	virtual void StartupModule() override
	{
		FModuleManager::GetModuleChecked<FDeviceRGBModule>("DeviceRGB").RegisterController(FCorsairController::Construct());
	}
};

IMPLEMENT_MODULE(FCorsairControllerModule, CorsairController)