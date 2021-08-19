// Copyright(c) 2021 Viktor Pramberg
using UnrealBuildTool;
using System.IO;

public class RazerController : ModuleRules
{
	public RazerController(ReadOnlyTargetRules Target) : base(Target)
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

		string SDKDir = Path.GetFullPath(Path.Combine(PluginDirectory, "Source", "ThirdParty", "ChromaSDK"));

		PublicIncludePaths.Add(Path.Combine(SDKDir, "include"));
		PublicDelayLoadDLLs.Add("RzChromaSDK64.dll");
		RuntimeDependencies.Add("$(ProjectDir)/Binaries/Win64/ChromaAppInfo.xml", Path.Combine(SDKDir, "ChromaAppInfo.xml"), StagedFileType.SystemNonUFS);
	}
}
