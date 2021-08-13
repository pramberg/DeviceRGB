// Copyright(c) 2021 Viktor Pramberg
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "IDeviceRGBController.h"
#include "IDeviceRGB.h"
#include "DeviceRGBSubsystem.generated.h"

class UTexture2D;
class UMaterialInterface;
class FDeviceRGBSceneViewExtension;

UENUM(BlueprintType)
enum class EDeviceRGBBlendMode : uint8
{
	AlphaBlend,
	Additive,
	Multiply,
	MAX UMETA(Hidden),
};

USTRUCT(BlueprintType)
struct FDeviceRGB_BackgroundLayerInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowedClasses = "Texture2D, MaterialInterface", DisallowedClasses = "TextureLightProfile"))
	UObject* Graphic;
};

USTRUCT(BlueprintType)
struct FDeviceRGBLayerInfo : public FDeviceRGB_BackgroundLayerInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Layer = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDeviceRGBBlendMode BlendMode = EDeviceRGBBlendMode::AlphaBlend;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FKey> Keys;
};

/**
 * A centralized point of interaction for all of DeviceRGB's features.
 */
UCLASS(meta = (DisplayName = "DeviceRGB Subsystem"))
class DEVICERGB_API UDeviceRGBSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	// Blueprint interface
	
	/**
	 * Sets a material as the current source of color on all devices.
	 *
	 * @param [in]	InMaterial	The material to use. Needs to be in the Post Process domain!
	 * @param 		InInfo	  	Layer information.
	 */
	UFUNCTION(BlueprintCallable, Category = "DeviceRGB", meta=(AutoCreateRefTerm="InInfo"))
	void SetMaterial(UMaterialInterface* InMaterial, const FDeviceRGBLayerInfo& InInfo);

	// Does nothing
	UFUNCTION(BlueprintCallable, Category = "DeviceRGB", meta = (AutoCreateRefTerm = "InInfo"))
	void SetTexture(UTexture2D* InTexture, const FDeviceRGBLayerInfo& InInfo);

	// ~Blueprint interface

public:
	decltype(auto) GetCurrentGraphic() const { return (CurrentGraphic); }

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// Only Windows is supported.
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	const TArray<FDeviceLEDInfo>& GetCachedInfos() const { return CachedLEDInfos; }

	void SetColors(TFunctionRef<void(IDeviceRGB*, TArray<FColor>&)> InFunction);

	void MarkCacheDirty()
	{
		bForceRefresh = true;
	}

	template<typename T>
	void RegisterController()
	{
		auto TempController = MakeUnique<T>();
		if (TempController->IsValid())
		{
			SupportedSDKs.Add(MoveTemp(TempController));
			MarkCacheDirty();
		}
	}

private:
	TVariant<UTexture2D*, UMaterialInterface*> CurrentGraphic;

	TSharedPtr<FDeviceRGBSceneViewExtension, ESPMode::ThreadSafe> ViewExtension;

	TArray<TUniquePtr<IDeviceRGBController>> SupportedSDKs;
	// Contains all connected devices' infos. 
	TArray<FDeviceLEDInfo> CachedLEDInfos;

	bool bForceRefresh = true;

	friend FDeviceRGBSceneViewExtension;
};
