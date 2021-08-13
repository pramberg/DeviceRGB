// Copyright(c) 2021 Viktor Pramberg
using UnrealBuildTool;
using System;
using System.IO;
using System.Collections.Generic;

public class DeviceRGBEditor : ModuleRules
{
	public DeviceRGBEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"Projects",
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"RenderCore",
				"Renderer",
				"RHI",
				"InputCore",
				"EngineSettings",
				"UnrealEd",
				"DeviceRGB",
				"AssetTools"
			}
		);
	}
}
