// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.

using UnrealBuildTool;

public class ReasonablePlanningAI : ModuleRules
{
   public ReasonablePlanningAI(ReadOnlyTargetRules Target) : base(Target)
   {
      PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
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
            "PropertyPath"
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
