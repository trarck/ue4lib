// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class YHLib : ModuleRules
{
	public YHLib(ReadOnlyTargetRules Target):base(Target)
	{
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
			new string[] {
				"YHLib/Public"
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				"YHLib/Private",
				// ... add other private include paths required here ...
			}
			);


        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core"
				// ... add other public dependencies that you statically link with here ...
			}
            );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",                             
				"Engine",
                "Slate",
                "SlateCore",
                "InputCore",
                "UMG",
				"AIModule",
                "RenderCore",
                "Renderer"
				// ... add private dependencies that you statically link with here ...	
			}
            );

        DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);

        if (Target.Type == TargetRules.TargetType.Editor)
        {
            PublicDependencyModuleNames.AddRange(new string[] {
                "Localization"
            });
        }

        //// Get the engine path. Ends with "Engine/"
        //string engine_path = Path.GetFullPath(BuildConfiguration.RelativeEnginePath);
        //// Now get the base of UE4's modules dir (could also be Developer, Editor, ThirdParty)
        //string srcrt_path = engine_path + "Source/Runtime/";
        //PublicIncludePaths.AddRange(
        //    new string[] {
        //               srcrt_path+"SlateCore/Private"
        //    }
        //);
	}
}
