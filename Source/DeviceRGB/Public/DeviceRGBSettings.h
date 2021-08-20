// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "DeviceRGBSettings.generated.h"

class UMaterialInterface;

/**
 * 
 */
UCLASS(config = Engine, defaultconfig)
class DEVICERGB_API UDeviceRGBSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UDeviceRGBSettings();

#if WITH_EDITOR

	virtual FText GetSectionText() const override;
	virtual FText GetSectionDescription() const override;

#endif // WITH_EDITOR

	/* Material to use when only specifying a texture. */
	UPROPERTY(config, EditAnywhere, Category = DeviceRGB)
	TSoftObjectPtr<UMaterialInterface> TextureMaterial;
};
