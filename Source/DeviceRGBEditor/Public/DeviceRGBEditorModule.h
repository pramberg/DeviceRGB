// Copyright(c) 2021 Viktor Pramberg
#pragma once
#include "Modules/ModuleManager.h"
#include <AssetTypeCategories.h>

class FDeviceRGBEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;

	static EAssetTypeCategories::Type GetAssetCategory() { return AssetCategory; }
private:
	static EAssetTypeCategories::Type AssetCategory;
};
