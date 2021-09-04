#pragma once
#include "CoreMinimal.h"
#include <InputCoreTypes.h>

class FWootingKeyConverter
{
public:
    static FKey ToFKey(const FIntPoint& InPoint)
    {
        return EKeys::Invalid;
    }
};