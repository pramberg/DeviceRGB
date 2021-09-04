// Copyright(c) 2021 Viktor Pramberg
#include "Modules/ModuleManager.h"
#include "TemplateController.h"
#include "DeviceRGB.h"

class FTemplateControllerModule : public IModuleInterface
{
public:
	virtual void StartupModule() override
	{
		FModuleManager::GetModuleChecked<FDeviceRGBModule>("DeviceRGB").RegisterController(FTemplateController::Construct());
	}
};

IMPLEMENT_MODULE(FTemplateControllerModule, TemplateController)