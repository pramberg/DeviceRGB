#include "Misc/AutomationTest.h"
#include "Tests/AutomationCommon.h"

#include "DeviceRGBSubsystem.h"
#include <Engine/Texture2D.h>
#include <Materials/Material.h>

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDeviceRGBSubsystemTest, "DeviceRGB.Subsystem",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::NonNullRHI |
	EAutomationTestFlags::ClientContext |
	EAutomationTestFlags::ProductFilter)

bool FDeviceRGBSubsystemTest::RunTest(const FString& Parameters)
{
	auto* Subsystem = GEngine->GetEngineSubsystem<UDeviceRGBSubsystem>();

	if (!Subsystem)
	{
		return false;
	}

	Subsystem->RemoveAllLayers();

	TestFalse("Can't add layer with null graphic", Subsystem->AddLayer({ nullptr }));

	TestTrue("Can add layer with material", Subsystem->AddLayer({ NewObject<UMaterial>() }));

	TestTrue("Can add layer with texture", Subsystem->AddLayer({ NewObject<UTexture2D>() }));

	AddExpectedError(TEXT("called with invalid graphic"));
	TestFalse("Can't add layer with static mesh", Subsystem->AddLayer({ NewObject<UStaticMesh>() }));

	TestEqual("Added two layers", Subsystem->GetNumberOfLayers(), 2);

	Subsystem->RemoveLayer();
	TestEqual("Can remove one layer", Subsystem->GetNumberOfLayers(), 1);

	Subsystem->RemoveAllLayers();
	TestEqual("Can remove all layers", Subsystem->GetNumberOfLayers(), 0);

	TestFalse("Subsystem is disabled when there are no layers", Subsystem->IsEnabled());
	
	return true;
}