// Copyright(c) 2021 Viktor Pramberg
#pragma once
#include "CoreMinimal.h"
#include "IDeviceRGBController.h"

class IDeviceRGB;

class FCorsairController : public IDeviceRGBController
{
public:
	virtual ~FCorsairController() override;
	static TUniquePtr<FCorsairController> Construct();

	virtual int32 GetNumberOfDevices() const override;
	virtual void FlushBuffers() override;

	static void FlushBuffersImpl();

	virtual void ForEachDevice(TFunctionRef<void(IDeviceRGB*)> InFunction) override;

	virtual void* GetDLLHandle() const override { return SDKHandle; }


	virtual void SetEnabled(bool bEnabled) override;

private:
	FCorsairController();

	void* SDKHandle = nullptr;
	TArray<TUniquePtr<IDeviceRGB>> Devices;
};