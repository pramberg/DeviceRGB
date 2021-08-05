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
#include "Corsair/CorsairDeviceSDK.h"
#include "IDevice.h"

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
	UWorld* World = GetTickableGameObjectWorld();
	if (!RenderTarget && World)
	{
		RenderTarget = NewObject<UTextureRenderTarget2D>(World);
		RenderTarget->InitCustomFormat(128, 64, EPixelFormat::PF_R8G8B8A8, false);

		Capture = NewObject<USceneCaptureComponent2D>(World);
		Capture->TextureTarget = RenderTarget;
		Capture->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;
		Capture->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
		/*Capture->bCaptureEveryFrame = false;
		Capture->bCaptureOnMovement = false;*/
	}

	if (CurrentGraphic.IsType<UMaterialInterface*>())
	{
		Capture->AddOrUpdateBlendable(CurrentGraphic.Get<UMaterialInterface*>(), 0.0f);
	}

	CurrentGraphic.Set<UMaterialInterface*>(InMaterial);

	if (!InMaterial)
	{
		return;
	}

	Capture->AddOrUpdateBlendable(InMaterial);
}

template<typename T>
void AddDeviceIfValid(TArray<TUniquePtr<IDeviceSDK>>& Collection)
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

	AddDeviceIfValid<FCorsairDeviceSDK>(SupportedSDKs);

	ViewExtension = FSceneViewExtensions::NewExtension<FDeviceRGBSceneViewExtension>(this);
}

void UDeviceRGBSubsystem::Deinitialize()
{
	Super::Deinitialize();

	ViewExtension.Reset();
}

void UDeviceRGBSubsystem::Tick(float DeltaTime)
{
	if (!Capture)
		return;

	Capture->CaptureSceneDeferred();

	if (!RenderTarget)
	{
		return;
	}

	int32 Width = RenderTarget->SizeX;
	int32 Height = RenderTarget->SizeY;

	FReadSurfaceDataFlags ReadSurfaceDataFlags;

	FRenderTarget* RTResourceRaw = RenderTarget->GameThread_GetRenderTargetResource();

	const int32 NumPixelsToRead = Width * Height;

	TArray<FColor> Pixels;
	Pixels.SetNumUninitialized(NumPixelsToRead);
	if (!RTResourceRaw || !RTResourceRaw->ReadPixelsPtr(Pixels.GetData(), ReadSurfaceDataFlags))
	{
		return;
	}

	for (auto& SDK : SupportedSDKs)
	{
		SDK->SetColors([&](IDevice* InDevice, TArray<FColor>& OutColors)
		{
			for (const auto& LEDInfo : InDevice->GetLEDInfos())
			{
				const int32 X2 = FMath::FloorToInt(LEDInfo.UV.X * Width);
				const int32 Y2 = FMath::FloorToInt(LEDInfo.UV.Y * Height);
				const FColor PixelColor = Pixels[Y2 * Width + X2];
				OutColors.Add(PixelColor);
			}
		});
	}
}

bool UDeviceRGBSubsystem::IsTickable() const
{
	
	//UE_LOG(LogTemp, Display, TEXT("Check tick %d"), GetCurrentGraphic().IsType<UMaterialInterface*>());
	return GetCurrentGraphic().IsType<UMaterialInterface*>();
}

TStatId UDeviceRGBSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UDeviceRGBSubsystem, STATGROUP_Tickables);
}

UWorld* UDeviceRGBSubsystem::GetTickableGameObjectWorld() const
{
	return GWorld;
}

bool UDeviceRGBSubsystem::IsTickableInEditor() const
{
	return true;
}

bool UDeviceRGBSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return PLATFORM_WINDOWS;
}
