// Copyright Troll Purse, Inc. All Rights Reserved.

namespace UnrealBuildTool.Rules
{
	public class ReasonablePlanningAINodes : ModuleRules
	{
		public ReasonablePlanningAINodes(ReadOnlyTargetRules Target) : base(Target)
		{
            PrivateIncludePaths.AddRange(
				new[] {
					"ReasonablePlanningAINodes/Private"
				}
			);

			PublicDependencyModuleNames.AddRange(
				new[] {
					"Core",
					"CoreUObject",
					"Engine",
					"GameplayTasks",
					"AIModule",
					"ReasonablePlanningAI",
					"Slate"
				}
			);

			PrivateDependencyModuleNames.AddRange(
				new[] {
                    "Core",
					"CoreUObject",
					"InputCore",
					"Slate",
					"Engine",
					"AssetTools",
					"UnrealEd", // for FAssetEditorManager
					"KismetWidgets",
					"KismetCompiler",
					"BlueprintGraph",
					"GraphEditor",
					"Kismet",  // for FWorkflowCentricApplication
					"PropertyEditor",
					"EditorStyle",
					"Slate",
					"SlateCore",
					"Sequencer",
					"DetailCustomizations",
					"Settings",
					"RenderCore",
                }
			);
		}
	}
}
