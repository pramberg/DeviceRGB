// Copyright(c) 2021 Viktor Pramberg
#include "TemplateController.h"
#include <Interfaces/IPluginManager.h>
#include "TemplateDevice.h"
#include "DeviceRGB.h"

FTemplateController::FTemplateController()
{
}

FTemplateController::~FTemplateController()
{
}

TUniquePtr<FTemplateController> FTemplateController::Construct()
{
    auto Controller = TUniquePtr<FTemplateController>(new FTemplateController());

    Controller->AddDevice<FTemplateDevice>();

    return MoveTemp(Controller);
}

void FTemplateController::FlushBuffers()
{
}

void FTemplateController::SetEnabled(bool bEnabled)
{
}
