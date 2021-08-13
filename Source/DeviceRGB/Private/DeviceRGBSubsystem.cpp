// Copyright(c) 2021 Viktor Pramberg
#include "DeviceRGBSubsystem.h"
#include <Engine/Texture2D.h>
#include <Materials/MaterialInterface.h>
#include <algorithm>
#include "DeviceRGBViewExtension.h"
#include <Engine/TextureRenderTarget2D.h>
#include <Components/SceneCaptureComponent2D.h>
#include <UnrealClient.h>
#include <TextureResource.h>
#include "IDeviceRGB.h"

// Need to be able to make layers where material only affects that layer. So say a base layer has one material, then another material is assigned to WASD as another layer.
void UDeviceRGBSubsystem::SetMaterial(UMaterialInterface* InMaterial, const FDeviceRGBLayerInfo& InInfo)
{
	if (!InMaterial)
	{
		return;
	}

	CurrentGraphic.Set<UMaterialInterface*>(InMaterial);
}

// This should probably be done using a default material, so we can do fancy features in the shader, like supersampling, and benefit from that when using textures too. Just make sure to only update the colors once, since they are persistent.
void UDeviceRGBSubsystem::SetTexture(UTexture2D* InTexture, const FDeviceRGBLayerInfo& InInfo)
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

void UDeviceRGBSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// This gets defined if there are internal controller modules available.
#ifdef DEVICERGB_CONTROLLERS_TO_LOAD

	for (FName Module : std::initializer_list<FName>{ DEVICERGB_CONTROLLERS_TO_LOAD })
	{
		FModuleManager::Get().LoadModuleChecked(Module);
	}

#endif // DEVICERGB_CONTROLLERS_TO_LOAD

	for (auto& SDK : SupportedSDKs)
	{
		SDK->ForEachDevice([&](IDeviceRGB* InDevice)
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

void UDeviceRGBSubsystem::SetColors(TFunctionRef<void(IDeviceRGB*, TArray<FColor>&)> InFunction)
{
	TArray<FColor> Colors;
	for (auto& SDK : SupportedSDKs)
	{
		SDK->ForEachDevice([&](IDeviceRGB* InDevice)
		{
			Colors.Empty(InDevice->GetNumLEDs());
			InFunction(InDevice, Colors);
			ensure(InDevice->GetNumLEDs() == Colors.Num());
			InDevice->SetColors(Colors, false);
		});
		SDK->FlushBuffers();
	}
}
