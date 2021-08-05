// Copyright(c) 2021 Viktor Pramberg
using UnrealBuildTool;
using System.IO;

public class DeviceRGB : ModuleRules
{
	public DeviceRGB(ReadOnlyTargetRules Target) : base(Target)
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
			}
		);

		PrivateIncludePathModuleNames.AddRange(
			new string[]
			{
				"Renderer"
			}
		);

		PrivateIncludePaths.AddRange(
			new string[]
			{
				Path.Combine(Path.GetFullPath(Target.RelativeEnginePath), "Source/Runtime/Renderer/Private/")

			}
		);

		if (Target.Platform == UnrealTargetPlatform.Win32 || Target.Platform == UnrealTargetPlatform.Win64)
		{
			string BaseLibraryPath = Path.GetFullPath(Path.Combine(ModuleDirectory, "..", "ThirdParty"));

			PublicIncludePaths.Add(Path.Combine(BaseLibraryPath, "CUESDK", "include"));
			PublicAdditionalLibraries.Add(Path.Combine(BaseLibraryPath, "CUESDK", "lib", "x64", "CUESDK.x64_2017.lib"));
			PublicDelayLoadDLLs.Add("CUESDK.x64_2017.dll");
		}
	}
}
