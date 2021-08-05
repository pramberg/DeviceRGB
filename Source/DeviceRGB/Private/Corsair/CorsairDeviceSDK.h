// Copyright(c) 2021 Viktor Pramberg
#pragma once
#include "CoreMinimal.h"
#include "IDeviceSDK.h"

class IDevice;

class FCorsairDeviceSDK : public IDeviceSDK
{
public:
	FCorsairDeviceSDK();
	virtual ~FCorsairDeviceSDK() override;

	virtual int32 GetNumberOfDevices() const override;
	virtual void FlushBuffers() override;

	static void FlushBuffersImpl();

	virtual void ForEachDevice(TFunctionRef<void(IDevice*)> InFunction) override;

	virtual bool IsValid() const override;
private:
	void* SDKHandle = nullptr;
	TArray<TUniquePtr<IDevice>> Devices;
};