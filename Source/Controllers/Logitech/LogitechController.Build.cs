// Copyright(c) 2021 Viktor Pramberg
using UnrealBuildTool;
using System.IO;

public class LogitechController : ModuleRules
{
	public LogitechController(ReadOnlyTargetRules Target) : base(Target)
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
				"EngineSettings",
				"DeviceRGB",
				"InputCore",
			}
		);

		string SDKDir = Path.GetFullPath(Path.Combine(PluginDirectory, "Source", "ThirdParty", "LED"));

		PublicIncludePaths.Add(Path.Combine(SDKDir, "Include"));
		PublicAdditionalLibraries.Add(Path.Combine(SDKDir, "Lib", "x64", "LogitechLEDLib.lib"));
	}
}
