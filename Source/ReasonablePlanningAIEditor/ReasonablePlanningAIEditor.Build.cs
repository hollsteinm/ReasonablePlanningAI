// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.

namespace UnrealBuildTool.Rules
{
   public class ReasonablePlanningAIEditor : ModuleRules
   {
      public ReasonablePlanningAIEditor(ReadOnlyTargetRules Target) : base(Target)
      {
			PrivateIncludePaths.AddRange(
		   new[] {
			   "ReasonablePlanningAIEditor/Private"
		   }
		);
			PublicDependencyModuleNames.AddRange(
            new[] {
               "Core",
               "Engine",
               "AIModule",
               "ReasonablePlanningAI",
               "InputCore"
            }
         );

         PrivateDependencyModuleNames.AddRange(
            new[] {
               "CoreUObject",
               "Engine",
               "Slate",
               "SlateCore",
               "UnrealEd",
               "PropertyEditor",
               "EditorStyle",
               "PropertyPath"
            }
         );
      }
   }
}