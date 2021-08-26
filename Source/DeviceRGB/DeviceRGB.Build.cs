// Copyright(c) 2021 Viktor Pramberg
using UnrealBuildTool;
using System;
using System.IO;
using System.Collections.Generic;

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
				"RenderCore",
				"Renderer",
				"RHI",
				"InputCore",
				"EngineSettings",
				"DeveloperSettings",
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

		// This forces a recompile if the ThirdParty directory is modified
		ExternalDependencies.Add(ThirdPartyDirectory);

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			TryAddController("CUESDK", "CorsairController");
			TryAddController("ChromaSDK", "RazerController");
			TryAddController("LED", "LogitechController");

			GenerateControllersToLoadDefinition();
		}
	}

	public string ThirdPartyDirectory
	{ 
		get { return Path.GetFullPath(Path.Combine(PluginDirectory, "Source", "ThirdParty")); } 
	}

    public List<string> ModulesToLoadOnStartup = new List<string>();

    /**
     * Attempts to add controller if an SDK is available.
     *
     * @param   InSDKName           Name of the SDK in the ThirdParty folder.
     * @param   InControllerModule  The name of the controller module.
     */
	public void TryAddController(string InSDKName, string InControllerModule)
    {
		string Dir = Path.Combine(ThirdPartyDirectory, InSDKName);
		bool SDKExists = Directory.Exists(Dir);
		if (SDKExists)
        {
			DynamicallyLoadedModuleNames.Add(InControllerModule);
			ModulesToLoadOnStartup.Add(string.Format("\"{0}\"", InControllerModule));
		}
	}

	/** Adds a new definition if there are any controller modules to load. */
	public void GenerateControllersToLoadDefinition()
    {
		if (ModulesToLoadOnStartup.Count > 0)
		{
			PrivateDefinitions.Add(string.Format("DEVICERGB_CONTROLLERS_TO_LOAD={0}", string.Join(", ", ModulesToLoadOnStartup)));
		}
	}
}
