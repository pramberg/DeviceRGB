// Copyright(c) 2021 Viktor Pramberg
#include "CorsairDeviceSDK.h"
#include <Interfaces/IPluginManager.h>
#include "CorsairDevice.h"

#include <CUESDK.h>

FCorsairDeviceSDK::FCorsairDeviceSDK()
{
	const FString BaseDir = IPluginManager::Get().FindPlugin("DeviceRGB")->GetBaseDir();
	const FString LibraryPath = FPaths::Combine(*BaseDir, TEXT("Source/ThirdParty/CUESDK/redist/x64/CUESDK.x64_2017.dll"));

	SDKHandle = FPlatformProcess::GetDllHandle(*LibraryPath);
	if (!SDKHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to find CUE DLL!!!!"));
		return;
	}

	CorsairPerformProtocolHandshake();
	if (CorsairGetLastError())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to connect to Corsair CUE service."));
		SDKHandle = nullptr;
		return;
	}

	for (int32 DeviceIndex = 0; DeviceIndex < GetNumberOfDevices(); DeviceIndex++)
	{
		Devices.Add(MakeUnique<FCorsairDevice>(DeviceIndex));
	}
}

FCorsairDeviceSDK::~FCorsairDeviceSDK()
{
	FPlatformProcess::FreeDllHandle(SDKHandle);
	SDKHandle = nullptr;
}

int32 FCorsairDeviceSDK::GetNumberOfDevices() const
{
	return CorsairGetDeviceCount();
}

void FCorsairDeviceSDK::FlushBuffers()
{
	FlushBuffersImpl();
}

void FCorsairDeviceSDK::FlushBuffersImpl()
{
	CorsairSetLedsColorsFlushBufferAsync([](void* Context, bool Result, CorsairError Error)
	{
		if (Error != CE_Success)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to flush color buffer"));
		}
	}, nullptr);
}

void FCorsairDeviceSDK::ForEachDevice(TFunctionRef<void(IDevice*)> InFunction)
{
	for (auto& Device : Devices)
	{
		InFunction(Device.Get());
	}
}

bool FCorsairDeviceSDK::IsValid() const
{
	return SDKHandle != nullptr;
}
