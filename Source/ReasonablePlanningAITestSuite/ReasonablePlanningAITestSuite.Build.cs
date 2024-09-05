// Copyright Troll Purse, Inc. All Rights Reserved.

namespace UnrealBuildTool.Rules
{
	public class ReasonablePlanningAITestSuite : ModuleRules
	{
		public ReasonablePlanningAITestSuite(ReadOnlyTargetRules Target) : base(Target)
		{
			PublicIncludePaths.AddRange(
				new string[] {
				}
			);

			PublicDependencyModuleNames.AddRange(
				new[] {
					"Core",
					"CoreUObject",
					"Engine",
					"GameplayTasks",
					"AIModule",
					"ReasonablePlanningAI"
                }
			);

            PrivateDependencyModuleNames.AddRange(
                new[] {
                    "UnrealEd",
                    "PropertyPath"
                }
            );

        }
	}
}
