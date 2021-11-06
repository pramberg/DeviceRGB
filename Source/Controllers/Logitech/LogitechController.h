// Copyright(c) 2021 Viktor Pramberg
#pragma once
#include "CoreMinimal.h"
#include "IDeviceRGBController.h"

#include "LogitechLEDLib.h"

class IDeviceRGB;

struct FLogitechColor
{
public:
	uint8 B, G, R, A;

	FLogitechColor(const FColor& InColor) :
		B(InColor.B),
		G(InColor.G),
		R(InColor.R),
		A(InColor.A)
	{}

	static int32 ToPercent(uint8 InValue)
	{
		return FMath::RoundToInt((InValue / 255.0f) * 100.0f);
	}

	int32 R_Percent() const { return ToPercent(R); }
	int32 G_Percent() const { return ToPercent(G); }
	int32 B_Percent() const { return ToPercent(B); }
};

static_assert(sizeof(FLogitechColor) == LOGI_LED_BITMAP_BYTES_PER_KEY, "Size of FLogitechColor must match Logitech's bytes per key");

class FLogitechController : public FDeviceRGBController
{
public:
	virtual ~FLogitechController() override;

	static TUniquePtr<FLogitechController> Construct();

	virtual void SetEnabled(bool bEnabled) override;

private:
	FLogitechController();
};