// Copyright Troll Purse, Inc. All Rights Reserved.

namespace UnrealBuildTool.Rules
{
	public class ReasonablePlanningAIEditor : ModuleRules
	{
		public ReasonablePlanningAIEditor(ReadOnlyTargetRules Target) : base(Target)
		{
			PublicIncludePaths.AddRange(
				new[] {
                	"ReasonablePlanningAIEditor/Public"
            	}
			);
				
		
			PrivateIncludePaths.AddRange(
				new[] {
                	"ReasonablePlanningAIEditor/Private"
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