// Copyright(c) 2021 Viktor Pramberg
#pragma once
#include "Modules/ModuleManager.h"
#include "DeviceRGBSubsystem.h"

class DEVICERGB_API FDeviceRGBModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;

	template<typename T>
	void RegisterController()
	{
		check(GEngine && GEngine->GetEngineSubsystem<UDeviceRGBSubsystem>());
		GEngine->GetEngineSubsystem<UDeviceRGBSubsystem>()->RegisterController<T>();
	}
};
