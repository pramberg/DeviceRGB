// Copyright(c) 2021 Viktor Pramberg
using UnrealBuildTool;
using System.IO;

public class TemplateController : ModuleRules
{
	public TemplateController(ReadOnlyTargetRules Target) : base(Target)
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

		string SDKDir = Path.GetFullPath(Path.Combine(PluginDirectory, "Source", "ThirdParty", "zzzSDKPathName"));

zzzIncludePathzzzDllPathzzzLibPathzzzDelayLoadDll
	}
}
