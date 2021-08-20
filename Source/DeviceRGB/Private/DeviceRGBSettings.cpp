// Fill out your copyright notice in the Description page of Project Settings.


#include "DeviceRGBSettings.h"
#include <Materials/MaterialInterface.h>

#define LOCTEXT_NAMESPACE "DeviceRGBSettings"

UDeviceRGBSettings::UDeviceRGBSettings()
{
	TextureMaterial = ConstructorHelpers::FObjectFinder<UMaterialInterface>(TEXT("/DeviceRGB/Materials/M_RGB_DefaultTexture.M_RGB_DefaultTexture")).Object;

	CategoryName = FName(TEXT("Plugins"));
	SectionName = TEXT("DeviceRGB");
}

#if WITH_EDITOR

FText UDeviceRGBSettings::GetSectionText() const
{
	return LOCTEXT("SectionText", "DeviceRGB");
}

FText UDeviceRGBSettings::GetSectionDescription() const
{
	return LOCTEXT("SectionDescription", "Project settings for DeviceRGB");
}

#endif // WITH_EDITOR

#undef LOCTEXT_NAMESPACE