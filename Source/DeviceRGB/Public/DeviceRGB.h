// Copyright(c) 2021 Viktor Pramberg
#pragma once
#include "Modules/ModuleManager.h"
#include "DeviceRGBSubsystem.h"

class DEVICERGB_API FDeviceRGBModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;

	void RegisterController(TUniquePtr<IDeviceRGBController>&& InController)
	{
		check(GEngine && GEngine->GetEngineSubsystem<UDeviceRGBSubsystem>());
		GEngine->GetEngineSubsystem<UDeviceRGBSubsystem>()->RegisterController(MoveTemp(InController));
	}
};
