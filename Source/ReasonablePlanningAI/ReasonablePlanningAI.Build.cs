// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ReasonablePlanningAI : ModuleRules
{
	public ReasonablePlanningAI(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[]
            {
                "ReasonablePlanningAI/Public"
            }
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[]
            {
                "ReasonablePlanningAI/Private"
            }
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"AIModule",
				"GameplayTags",
				"GameplayTasks",
				"NavigationSystem",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore"
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
