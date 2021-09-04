// Copyright(c) 2021 Viktor Pramberg
#pragma once
#include "CoreMinimal.h"
#include "IDeviceRGBController.h"

class IDeviceRGB;

class FCorsairController : public FDeviceRGBController
{
public:
	virtual ~FCorsairController() override;
	static TUniquePtr<FCorsairController> Construct();

	virtual void FlushBuffers() override;
	static void FlushBuffersImpl();
	virtual void* GetDLLHandle() const override { return SDKHandle; }
	virtual void SetEnabled(bool bEnabled) override;

private:
	FCorsairController();

	void* SDKHandle = nullptr;
};