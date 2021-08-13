// Copyright(c) 2021 Viktor Pramberg
#include "DeviceRGBEditorModule.h"
#include <AssetToolsModule.h>
#include <IAssetTools.h>

#define LOCTEXT_NAMESPACE "DeviceRGBEditorModule"

IMPLEMENT_MODULE(FDeviceRGBEditorModule, DeviceRGBEditor)

EAssetTypeCategories::Type FDeviceRGBEditorModule::AssetCategory;

void FDeviceRGBEditorModule::StartupModule()
{
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	AssetCategory = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("DeviceRGB")), LOCTEXT("AssetCategory", "DeviceRGB"));
}

#undef LOCTEXT_NAMESPACE