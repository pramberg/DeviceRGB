// Copyright(c) 2021 Viktor Pramberg
using UnrealBuildTool;

public class DeviceRGBTests : ModuleRules
{
	public DeviceRGBTests(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"InputCore",
				"DeviceRGB"
			}
		);
	}
}
