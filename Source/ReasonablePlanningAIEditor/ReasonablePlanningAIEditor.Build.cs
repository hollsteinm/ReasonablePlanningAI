// Copyright Troll Purse, Inc. All Rights Reserved.

namespace UnrealBuildTool.Rules
{
	public class ReasonablePlanningAIEditor : ModuleRules
	{
		public ReasonablePlanningAIEditor(ReadOnlyTargetRules Target) : base(Target)
		{
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