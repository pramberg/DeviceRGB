// Copyright(c) 2021 Viktor Pramberg
#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "DeviceRGBMaterialFactory.generated.h"

/**
 * Factory for creating materials with correct settings for DeviceRGB.
 */
UCLASS()
class UDeviceRGBMaterialFactory : public UFactory
{
	GENERATED_BODY()
	
public:
	UDeviceRGBMaterialFactory();
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	virtual FText GetDisplayName() const override;
	virtual uint32 GetMenuCategories() const override;
	virtual FText GetToolTip() const override;
	virtual FString GetDefaultNewAssetName() const override;
};
