// Copyright(c) 2021 Viktor Pramberg
#include "DeviceRGB.h"
#include <Interfaces/IPluginManager.h>
	
IMPLEMENT_MODULE(FDeviceRGBModule, DeviceRGB)

void FDeviceRGBModule::StartupModule()
{
	const FString ShaderDirectory = FPaths::Combine(IPluginManager::Get().FindPlugin("DeviceRGB")->GetBaseDir(), TEXT("Shaders"));
	AddShaderSourceDirectoryMapping("/DeviceRGB", ShaderDirectory);
}
