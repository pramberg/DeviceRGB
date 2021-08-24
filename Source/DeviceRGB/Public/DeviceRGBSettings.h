// Copyright(c) 2021 Viktor Pramberg
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
	
private:
	static constexpr auto* DefaultProjectNameOverride = TEXT("DeviceRGB");

public:
	UDeviceRGBSettings();

	FString GetProjectName() const;

#if WITH_EDITOR

	virtual FText GetSectionText() const override;
	virtual FText GetSectionDescription() const override;
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

#endif // WITH_EDITOR

	/* Material to use when only specifying a texture. */
	UPROPERTY(config, EditAnywhere, Category = Visuals)
	TSoftObjectPtr<UMaterialInterface> TextureMaterial;

	/**   
	  * Check this to use the project name (or the executable name if the project name doesn't exist) when registering with various SDKs.
	  * If you choose to enable this, the name may show up in their applications. For example, Logitech has an "Integrations" page, where this name would
	  * show up. It is recommended to enable this if you feel comfortable.
	  */
	UPROPERTY(config, EditAnywhere, Category = Privacy)
	bool bAllowRegisteringProjectName = false;

	/** The name to use if bAllowRegisteringProjectName is false. Can't be empty. */
	UPROPERTY(config, EditAnywhere, Category = Privacy, meta = (EditCondition = "!bAllowRegisteringProjectName"))
	FString ProjectNameOverride = DefaultProjectNameOverride;


};
