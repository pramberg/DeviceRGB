// Copyright(c) 2021 Viktor Pramberg
using UnrealBuildTool;
using System.IO;

public class CorsairController : ModuleRules
{
	public CorsairController(ReadOnlyTargetRules Target) : base(Target)
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

		string SDKDir = Path.GetFullPath(Path.Combine(PluginDirectory, "Source", "ThirdParty", "CUESDK"));

		PublicIncludePaths.Add(Path.Combine(SDKDir, "include"));
		RuntimeDependencies.Add(Path.Combine(SDKDir, "redist", "x64", "CUESDK.x64_2017.dll"));
		PublicAdditionalLibraries.Add(Path.Combine(SDKDir, "lib", "x64", "CUESDK.x64_2017.lib"));
		PublicDelayLoadDLLs.Add("CUESDK.x64_2017.dll");
	}
}
