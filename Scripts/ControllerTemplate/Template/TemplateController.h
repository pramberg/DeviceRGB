// Copyright(c) 2021 Viktor Pramberg
#pragma once
#include "CoreMinimal.h"
#include "IDeviceRGBController.h"

class IDeviceRGB;

class FTemplateController : public FDeviceRGBController
{
public:
    static TUniquePtr<FTemplateController> Construct();

    virtual ~FTemplateController() override;
    virtual void FlushBuffers() override;
    virtual void SetEnabled(bool bEnabled) override;

private:
    FTemplateController();
};