// Copyright(c) 2021 Viktor Pramberg
#include "DeviceRGBMaterialFactory.h"
#include <Materials/Material.h>
#include <AssetTypeCategories.h>
#include "DeviceRGBEditorModule.h"

#define LOCTEXT_NAMESPACE "DeviceRGBMaterialFactory"

UDeviceRGBMaterialFactory::UDeviceRGBMaterialFactory()
{
	SupportedClass = UMaterial::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UDeviceRGBMaterialFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	UMaterial* Material = NewObject<UMaterial>(InParent, InClass, InName, Flags);

	Material->MaterialDomain = EMaterialDomain::MD_PostProcess;
	Material->BlendableOutputAlpha = true;
	Material->PostEditChange();

	return Material;
}

FText UDeviceRGBMaterialFactory::GetDisplayName() const
{
	return LOCTEXT("DisplayName", "DeviceRGB Material");
}

uint32 UDeviceRGBMaterialFactory::GetMenuCategories() const
{
	return FDeviceRGBEditorModule::GetAssetCategory();
}

FText UDeviceRGBMaterialFactory::GetToolTip() const
{
	return LOCTEXT("Tooltip", "Creates a material with the correct settings to work with DeviceRGB out of the box.");
}

FString UDeviceRGBMaterialFactory::GetDefaultNewAssetName() const
{
	return TEXT("M_RGB_NewMaterial");
}

#undef LOCTEXT_NAMESPACE