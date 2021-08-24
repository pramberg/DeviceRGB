// Copyright(c) 2021 Viktor Pramberg
#include "DeviceRGBSettings.h"
#include "DeviceRGB.h"
#include <Materials/MaterialInterface.h>
#include <GeneralProjectSettings.h>

#define LOCTEXT_NAMESPACE "DeviceRGBSettings"

UDeviceRGBSettings::UDeviceRGBSettings()
{
	TextureMaterial = ConstructorHelpers::FObjectFinder<UMaterialInterface>(TEXT("/DeviceRGB/Materials/M_RGB_DefaultTexture.M_RGB_DefaultTexture")).Object;

	CategoryName = FName(TEXT("Plugins"));
	SectionName = TEXT("DeviceRGB");
}

FString UDeviceRGBSettings::GetProjectName() const
{
	FString Name = ProjectNameOverride;

	if (bAllowRegisteringProjectName)
	{
		const auto* ProjectSettings = GetDefault<UGeneralProjectSettings>();
		Name = ProjectSettings->ProjectName.IsEmpty() ? FApp::GetName() : ProjectSettings->ProjectName;
	}
	else
	{
		if (Name.IsEmpty())
		{
			ensureMsgf(false, TEXT("ProjectNameOverride can't be empty. Using default value."));
			Name = DefaultProjectNameOverride;
		}
	}

	// The editor exe may get registered as a different entry than a packaged build, so ensure they have different names.
	if constexpr (WITH_EDITOR)
	{
		Name.Append(TEXT("-Editor"));
	}

	return Name;
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

void UDeviceRGBSettings::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UDeviceRGBSettings, ProjectNameOverride))
	{
		if (bAllowRegisteringProjectName)
		{
			return;
		}

		if (ProjectNameOverride.IsEmpty())
		{
			UE_LOG(LogDeviceRGB, Error, TEXT("ProjectNameOverride can't be empty. Resetting it to default value."));
			ProjectNameOverride = DefaultProjectNameOverride;
		}
	}
}

#endif // WITH_EDITOR

#undef LOCTEXT_NAMESPACE