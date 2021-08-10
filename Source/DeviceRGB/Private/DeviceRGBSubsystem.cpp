// Copyright(c) 2021 Viktor Pramberg
#include "DeviceRGBSubsystem.h"
#include <Engine/Texture2D.h>
#include <Materials/MaterialInterface.h>
#include <CUESDK.h>
#include <algorithm>
#include "DeviceRGBViewExtension.h"
#include <Engine/TextureRenderTarget2D.h>
#include <Components/SceneCaptureComponent2D.h>
#include <UnrealClient.h>
#include <TextureResource.h>
#include "IDevice.h"

#include "Corsair/CorsairDeviceSDK.h"
#include "Razer/RazerDeviceSDK.h"

// This should probably be done using a default material, so we can do fancy features in the shader, like supersampling, and benefit from that when using textures too. Just make sure to only update the colors once, since they are persistent.
void UDeviceRGBSubsystem::SetTexture(UTexture2D* InTexture)
{
	/*if (!InTexture)
		return;

	TextureCompressionSettings OldCompressionSettings = InTexture->CompressionSettings; TextureMipGenSettings OldMipGenSettings = InTexture->MipGenSettings; bool OldSRGB = InTexture->SRGB;

	InTexture->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
	InTexture->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps;
	InTexture->SRGB = false;
	InTexture->UpdateResource();

	const FColor* FormatedImageData = static_cast<const FColor*>(InTexture->PlatformData->Mips[0].BulkData.LockReadOnly());

	for (auto& SDK : SupportedSDKs)
	{
		SDK->SetColors([&](IDevice* InDevice, TArray<FColor>& OutColors)
		{
			for (const auto& LEDInfo : InDevice->GetLEDInfos())
			{
				int32 X = FMath::Floor(LEDInfo.UV.X * InTexture->GetSizeX());
				int32 Y = FMath::Floor(LEDInfo.UV.Y * InTexture->GetSizeY());
				FColor PixelColor = FormatedImageData[Y * InTexture->GetSizeX() + X];
				OutColors.Add(PixelColor);
			}
		});
	}

	InTexture->PlatformData->Mips[0].BulkData.Unlock();

	InTexture->CompressionSettings = OldCompressionSettings;
	InTexture->MipGenSettings = OldMipGenSettings;
	InTexture->SRGB = OldSRGB;
	InTexture->UpdateResource();
	
	CurrentGraphic.Set<UTexture2D*>(InTexture);*/
}
// Need to be able to make layers where material only affects that layer. So say a base layer has one material, then another material is assigned to WASD as another layer.
void UDeviceRGBSubsystem::SetMaterial(UMaterialInterface* InMaterial)
{
	if (!InMaterial)
	{
		return;
	}

	CurrentGraphic.Set<UMaterialInterface*>(InMaterial);
}

template<typename T>
void AddSDKIfValid(TArray<TUniquePtr<IDeviceSDK>>& Collection)
{
	auto Temp = MakeUnique<T>();
	if (Temp->IsValid())
	{
		Collection.Add(MoveTemp(Temp));
	}
}

void UDeviceRGBSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	AddSDKIfValid<FCorsairDeviceSDK>(SupportedSDKs);
	AddSDKIfValid<FRazerDeviceSDK>(SupportedSDKs);

	for (auto& SDK : SupportedSDKs)
	{
		SDK->ForEachDevice([&](IDevice* InDevice)
		{
			for (const auto& LEDInfo : InDevice->GetLEDInfos())
			{
				CachedLEDInfos.Add(LEDInfo);
			}
		});
	}

	ViewExtension = FSceneViewExtensions::NewExtension<FDeviceRGBSceneViewExtension>(this);
}

void UDeviceRGBSubsystem::Deinitialize()
{
	Super::Deinitialize();

	ViewExtension.Reset();
}

bool UDeviceRGBSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return PLATFORM_WINDOWS;
}
