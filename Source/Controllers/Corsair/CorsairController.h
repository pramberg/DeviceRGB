// Copyright(c) 2021 Viktor Pramberg
#pragma once
#include "CoreMinimal.h"
#include "IDeviceRGBController.h"

class IDeviceRGB;

class FCorsairController : public IDeviceRGBController
{
public:
	FCorsairController();
	virtual ~FCorsairController() override;

	virtual int32 GetNumberOfDevices() const override;
	virtual void FlushBuffers() override;

	static void FlushBuffersImpl();

	virtual void ForEachDevice(TFunctionRef<void(IDeviceRGB*)> InFunction) override;

	virtual bool IsValid() const override;
private:
	void* SDKHandle = nullptr;
	TArray<TUniquePtr<IDeviceRGB>> Devices;
};