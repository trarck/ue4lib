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
	}
}
