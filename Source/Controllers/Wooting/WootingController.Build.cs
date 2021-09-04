// Copyright(c) 2021 Viktor Pramberg
using UnrealBuildTool;
using System.IO;

public class WootingController : ModuleRules
{
	public WootingController(ReadOnlyTargetRules Target) : base(Target)
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
				"DeviceRGB",
				"InputCore",
			}
		);

		string SDKDir = Path.GetFullPath(Path.Combine(PluginDirectory, "Source", "ThirdParty", "Wooting"));

		PublicIncludePaths.Add(Path.Combine(SDKDir, "include"));
		RuntimeDependencies.Add(Path.Combine(SDKDir, "redist/x64/wooting-rgb-sdk64.dll"));
		PublicAdditionalLibraries.Add(Path.Combine(SDKDir, "lib/x64/wooting-rgb-sdk64.lib"));
		PublicDelayLoadDLLs.Add("wooting-rgb-sdk64.dll");

	}
}
