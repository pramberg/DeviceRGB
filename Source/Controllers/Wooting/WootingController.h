// Copyright(c) 2021 Viktor Pramberg
#pragma once
#include "CoreMinimal.h"
#include "IDeviceRGBController.h"

class IDeviceRGB;

struct FWootingColor
{
public:
	uint8 R, G, B;

	FWootingColor(const FColor& InColor) :
		R(InColor.R),
		G(InColor.G),
		B(InColor.B)
	{}
};

class FWootingController : public FDeviceRGBController
{
public:
    static TUniquePtr<FWootingController> Construct();

    virtual ~FWootingController() override;
    virtual void FlushBuffers() override;
	static void FlushBuffersImpl();
    virtual void SetEnabled(bool bEnabled) override;

private:
    FWootingController();

	void* SDKHandle = nullptr;
};