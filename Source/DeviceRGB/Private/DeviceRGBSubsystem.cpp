// Copyright(c) 2021 Viktor Pramberg
#include "DeviceRGBSubsystem.h"
#include "DeviceRGBViewExtension.h"
#include "DeviceRGBSettings.h"
#include "IDeviceRGB.h"
#include "DeviceRGB.h"

#include <Engine/Texture2D.h>
#include <Materials/MaterialInterface.h>
#include <Materials/MaterialInstanceDynamic.h>
#include <Algo/Transform.h>

bool UDeviceRGBSubsystem::AddLayer(const FDeviceRGBLayerInfo& InInfo)
{
	if (auto Cache = CreateGraphicsCache(InInfo))
	{
		GraphicsCaches.Add(Cache.GetValue());
		SetEnabled(true);
		MarkCacheDirty();
		return true;
	}
	return false;
}

void UDeviceRGBSubsystem::RemoveLayer()
{
	if (GraphicsCaches.Num() == 0)
	{
		return;
	}

	GraphicsCaches.RemoveAt(GraphicsCaches.Num() - 1);
	MarkCacheDirty();

	if (GraphicsCaches.Num() == 0)
	{
		SetEnabled(false);
	}
}

void UDeviceRGBSubsystem::RemoveAllLayers()
{
	GraphicsCaches.Empty();
	MarkCacheDirty();
	SetEnabled(false);
}

bool UDeviceRGBSubsystem::InsertLayer(int32 InIndex, const FDeviceRGBLayerInfo& InInfo)
{
	if (auto Cache = CreateGraphicsCache(InInfo))
	{
		GraphicsCaches.Insert(Cache.GetValue(), InIndex);
		SetEnabled(true);
		MarkCacheDirty();
		return true;
	}
	return false;
}

bool UDeviceRGBSubsystem::ReplaceLayer(int32 InIndex, const FDeviceRGBLayerInfo& InInfo)
{
	if (!GraphicsCaches.IsValidIndex(InIndex))
	{
		return false;
	}

	if (auto Cache = CreateGraphicsCache(InInfo))
	{
		GraphicsCaches[InIndex] = Cache.GetValue();
		SetEnabled(true);
		MarkCacheDirty();
		return true;
	}
	return false;
}

void UDeviceRGBSubsystem::SetEnabled(bool bEnabled)
{
	if (bIsEnabled == bEnabled)
	{
		return;
	}

	bIsEnabled = bEnabled;
	for (auto& SDK : SupportedSDKs)
	{
		SDK->SetEnabled(bIsEnabled);
	}
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

	TInterval<int32> CurrentRange{ 0, 0 };
	ForEachDevice([&](IDeviceRGB* InDevice)
	{
		CurrentRange.Min = CurrentRange.Max;
		for (const auto& LEDInfo : InDevice->GetLEDInfos())
		{
			CachedLEDInfos.Add(LEDInfo);
			CurrentRange.Max++;
		}
		DeviceRanges.Add(CurrentRange);
	});

	ViewExtension = FSceneViewExtensions::NewExtension<FDeviceRGBSceneViewExtension>(this);
}

void UDeviceRGBSubsystem::Deinitialize()
{
	Super::Deinitialize();

	ViewExtension.Reset();
}

bool UDeviceRGBSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return PLATFORM_WINDOWS && !IsRunningCommandlet();
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
			check(InDevice->GetNumLEDs() == Colors.Num());
			InDevice->SetColors(Colors, false);
		});
		SDK->FlushBuffers();
	}
}

void UDeviceRGBSubsystem::MarkCacheDirty()
{
	bForceRefresh = true;
}

void UDeviceRGBSubsystem::RegisterController(TUniquePtr<IDeviceRGBController>&& InController)
{
	if (InController)
	{
		SupportedSDKs.Add(MoveTemp(InController));
		MarkCacheDirty();
	}
}

void UDeviceRGBSubsystem::ForEachDevice(TFunctionRef<void(IDeviceRGB*)> InFunction)
{
	for (auto& SDK : SupportedSDKs)
	{
		SDK->ForEachDevice(InFunction);
	}
}

void UDeviceRGBSubsystem::ForEachDevice(TFunctionRef<void(IDeviceRGB*, int32)> InFunction)
{
	int32 i = 0;
	for (auto& SDK : SupportedSDKs)
	{
		SDK->ForEachDevice([&](IDeviceRGB* InDevice)
		{
			InFunction(InDevice, i++);
		});
	}
}

TOptional<FDeviceRGBGraphicCache> UDeviceRGBSubsystem::CreateGraphicsCache(const FDeviceRGBLayerInfo& InInfo)
{
	if (!InInfo.Graphic)
	{
		return TOptional<FDeviceRGBGraphicCache>();
	}

	FDeviceRGBGraphicCache Cache;
	if (UMaterialInterface* Material = Cast<UMaterialInterface>(InInfo.Graphic))
	{
		Cache.Material.Reset(Material);
	}
	else if (UTexture2D* Texture = Cast<UTexture2D>(InInfo.Graphic))
	{
		UMaterialInterface* BaseMaterial = GetDefault<UDeviceRGBSettings>()->TextureMaterial.LoadSynchronous();

		const FName Name(FString::Printf(TEXT("%s(%s)"), *BaseMaterial->GetName(), *Texture->GetName()));
		UMaterialInstanceDynamic* CreatedMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, nullptr, Name);

		CreatedMaterial->SetTextureParameterValue(TEXT("Texture"), Texture);

		Cache.Material.Reset(CreatedMaterial);
	}
	else
	{
		UE_LOG(LogDeviceRGB, Error, TEXT("UDeviceRGBSubsystem::InitializeGraphicsCache() called with invalid graphic: [%s]. DeviceRGB only supports Materials and Texture2D!"), *InInfo.Graphic->GetName());
		return TOptional<FDeviceRGBGraphicCache>();
	}
	Cache.BlendMode = InInfo.BlendMode;

	if (InInfo.DeviceTypes != 0)
	{
		ForEachDevice([this, &Cache, Type = InInfo.DeviceTypes](IDeviceRGB* InDevice, int32 InCurrentIndex)
		{
			if (Type & (1 << static_cast<uint8>(InDevice->GetType())))
			{
				const TInterval<int32>& Interval = DeviceRanges[InCurrentIndex];
				for (int32 i = Interval.Min; i < Interval.Max; i++)
				{
					Cache.Indices.Add(i);
				}
			}
		});
	}

	if (InInfo.Keys.Num())
	{
		ForEachDevice([this, &Cache, &InInfo](IDeviceRGB* InDevice, int32 InCurrentIndex)
		{
			const auto Indices = InDevice->GetIndicesForKeys(InInfo.Keys);
			if (Indices.Num())
			{
				const int32 DeviceStartIndex = DeviceRanges[InCurrentIndex].Min;
				Cache.Indices.Reserve(Indices.Num());
				Algo::Transform(Indices, Cache.Indices, [DeviceStartIndex](int32 DeviceIndex) { return DeviceStartIndex + DeviceIndex; });
			}
		});
	}

	return Cache;
}
