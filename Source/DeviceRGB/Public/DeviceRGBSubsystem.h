// Copyright(c) 2021 Viktor Pramberg
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "IDeviceRGBController.h"
#include "IDeviceRGB.h"
#include <UObject/StrongObjectPtr.h>
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

/**
 * Contains information for a single layer. This information gets processed and turned into a graphics cache, that can be consumed efficiently by the view extension.
 */
USTRUCT(BlueprintType)
struct FDeviceRGBLayerInfo
{
	GENERATED_BODY()

public:
	// The material or texture to use for this layer
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowedClasses = "Texture2D, MaterialInterface", DisallowedClasses = "TextureLightProfile"))
	UObject* Graphic;

	// The blend mode to use for this layer
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDeviceRGBBlendMode BlendMode = EDeviceRGBBlendMode::AlphaBlend;

	// Keys to isolate this layer to. If empty, the layer will apply to all keys.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FKey> Keys;

	// What devices to apply this layer to. If none are specified, layer will be applied to all devices.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bitmask, BitmaskEnum = EDeviceRGBType))
	int32 DeviceTypes;
};

/** Contains the processed layer info, that the view extension can deal with efficiently. */
struct FDeviceRGBGraphicCache
{
	TStrongObjectPtr<UMaterialInterface> Material;
	TArray<int32> Indices;
	EDeviceRGBBlendMode BlendMode;
};

/**
 * A centralized point of interaction for all of DeviceRGB's features.
 */
UCLASS(meta = (DisplayName = "DeviceRGB Subsystem"))
class DEVICERGB_API UDeviceRGBSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	/**
	 * Adds a layer on the top of the layer stack. Will enable the subsystem if it was disabled!
	 *
	 * @param 	InInfo	Layer information.
	 * 					
	 * @returns	True if a layer was added successfully.
	 */
	UFUNCTION(BlueprintCallable, Category = "DeviceRGB", meta = (AutoCreateRefTerm = "InInfo"))
	bool AddLayer(const FDeviceRGBLayerInfo& InInfo);

	/**
	 * Inserts a layer into the layer stack. If InIndex >= NumLayers, it will just add the layer. 
	 *
	 * @param 	InIndex	The index to insert at.
	 * @param 	InInfo 	Layer information.
	 *
	 * @returns	True if a layer was inserted successfully.
	 */
	UFUNCTION(BlueprintCallable, Category = "DeviceRGB", meta = (AutoCreateRefTerm = "InInfo"))
	bool InsertLayer(int32 InIndex, const FDeviceRGBLayerInfo& InInfo);

	/**
	 * Replaces an existing layer with another layer. Will not do anything if InIndex >= NumLayers.
	 *
	 * @param 	InIndex	The index to replace.
	 * @param 	InInfo 	Layer information.
	 *
	 * @returns	True if a layer was replaced successfully.
	 */
	UFUNCTION(BlueprintCallable, Category = "DeviceRGB", meta = (AutoCreateRefTerm = "InInfo"))
	bool ReplaceLayer(int32 InIndex, const FDeviceRGBLayerInfo& InInfo);

	/** Removes the top layer from the layer stack. Will disable the subsystem if all layers are removed. */
	UFUNCTION(BlueprintCallable, Category = "DeviceRGB")
	void RemoveLayer();

	/** Removes all layers, and disables the subsystem. */
	UFUNCTION(BlueprintCallable, Category = "DeviceRGB")
	void RemoveAllLayers();

	/**
	 * Gets number of layers that are currently active.
	 */
	UFUNCTION(BlueprintPure, Category = "DeviceRGB")
	int32 GetNumberOfLayers() const { return GraphicsCaches.Num(); }

	/**
	 * Enables or disables this subsystem. Disabling means control is released back to the device.
	 *
	 * @param 	bEnabled	True to enable, false to disable.
	 */
	UFUNCTION(BlueprintCallable, Category = "DeviceRGB")
	void SetEnabled(bool bEnabled);

	/**
	 * Query if DeviceRGB is enabled.
	 */
	UFUNCTION(BlueprintPure, Category = "DeviceRGB")
	bool IsEnabled() const { return bIsEnabled; }

public:
	//~ Begin UEngineSubsystem Interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	//~ End UEngineSubsystem Interface

	const auto& GetCachedInfos() const { return CachedLEDInfos; }
	const auto& GetGraphicsCaches() const { return (GraphicsCaches); }

	void SetColors(TFunctionRef<void(IDeviceRGB*, TArray<FColor>&)> InFunction);

	void MarkCacheDirty();
	void RegisterController(TUniquePtr<IDeviceRGBController>&& InController);
	void ForEachDevice(TFunctionRef<void(IDeviceRGB*)> InFunction);
	void ForEachDevice(TFunctionRef<void(IDeviceRGB*, int32)> InFunction);

private:
	TOptional<FDeviceRGBGraphicCache> CreateGraphicsCache(const FDeviceRGBLayerInfo& InInfo);

private:
	TArray<FDeviceRGBGraphicCache> GraphicsCaches;

	TSharedPtr<FDeviceRGBSceneViewExtension, ESPMode::ThreadSafe> ViewExtension;

	TArray<TUniquePtr<IDeviceRGBController>> SupportedSDKs;
	// Contains all connected devices' infos. 
	TArray<FDeviceLEDInfo> CachedLEDInfos;

	// Contains the range of each device in the cached LED infos array. [Min, Max)
	TArray<TInterval<int32>> DeviceRanges;

	bool bForceRefresh = true;
	bool bIsEnabled = true;

	friend FDeviceRGBSceneViewExtension;
};
