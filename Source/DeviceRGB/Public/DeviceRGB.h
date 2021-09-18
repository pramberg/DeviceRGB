// Copyright(c) 2021 Viktor Pramberg
#pragma once
#include "Modules/ModuleManager.h"
#include "DeviceRGBSubsystem.h"

DEVICERGB_API DECLARE_LOG_CATEGORY_EXTERN(LogDeviceRGB, Log, All)

class DEVICERGB_API FDeviceRGBModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;

	void RegisterController(TUniquePtr<IDeviceRGBController>&& InController)
	{
		check(GEngine);

		if (UDeviceRGBSubsystem* Subsystem = GEngine->GetEngineSubsystem<UDeviceRGBSubsystem>())
		{
			Subsystem->RegisterController(MoveTemp(InController));
		}
	}
};
