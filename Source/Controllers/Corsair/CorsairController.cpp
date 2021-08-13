// Copyright(c) 2021 Viktor Pramberg
#include "CorsairController.h"
#include <Interfaces/IPluginManager.h>
#include "CorsairDevice.h"

#include <CUESDK.h>

FCorsairController::FCorsairController()
{
	const FString BaseDir = IPluginManager::Get().FindPlugin("DeviceRGB")->GetBaseDir();
	const FString LibraryPath = FPaths::Combine(*BaseDir, TEXT("Source/ThirdParty/CUESDK/redist/x64/CUESDK.x64_2017.dll"));

	SDKHandle = FPlatformProcess::GetDllHandle(*LibraryPath);
	if (!SDKHandle)
	{
		return;
	}

	CorsairPerformProtocolHandshake();
	if (CorsairGetLastError())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to connect to Corsair CUE service."));
		FPlatformProcess::FreeDllHandle(SDKHandle);
		SDKHandle = nullptr;
		return;
	}

	for (int32 DeviceIndex = 0; DeviceIndex < GetNumberOfDevices(); DeviceIndex++)
	{
		Devices.Add(MakeUnique<FCorsairDevice>(DeviceIndex));
	}
}

FCorsairController::~FCorsairController()
{
	FPlatformProcess::FreeDllHandle(SDKHandle);
	SDKHandle = nullptr;
}

int32 FCorsairController::GetNumberOfDevices() const
{
	return CorsairGetDeviceCount();
}

void FCorsairController::FlushBuffers()
{
	FlushBuffersImpl();
}

void FCorsairController::FlushBuffersImpl()
{
	CorsairSetLedsColorsFlushBufferAsync([](void* Context, bool Result, CorsairError Error)
	{
		if (Error != CE_Success)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to flush color buffer"));
		}
	}, nullptr);
}

void FCorsairController::ForEachDevice(TFunctionRef<void(IDeviceRGB*)> InFunction)
{
	for (auto& Device : Devices)
	{
		InFunction(Device.Get());
	}
}

bool FCorsairController::IsValid() const
{
	return SDKHandle != nullptr;
}
