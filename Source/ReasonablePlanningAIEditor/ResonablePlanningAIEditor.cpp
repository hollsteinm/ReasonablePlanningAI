// Copyright Troll Purse. All Rights Reserved.

#include "ReasonablePlanningAIEditor.h"
#include "StateKeyValueReferenceCustom.h"
#include "Core/RpaiTypes.h"
#include "ComposerBehaviorAssetTypeActions.h"

#define LOCTEXT_NAMESPACE "ResonablePlanningAIEditor"

static const FName RpaiMenuCategoryKey(TEXT("Reasonable Planning AI"));
static const FText RpaiMenuCategoryText(NSLOCTEXT("RpaiEditor", "AssetCategory", "Reasonable Planning AI"));

struct FReasonablePlanningAIEditorModule : public IReasonablePlanningAIEditorModule
{
	EAssetTypeCategories::Type ModuleAssetCategoryBit;
	TSharedPtr<FComposerBehaviorAssetTypeActions> ComposerBehaviorAssetTypeActions;

	FReasonablePlanningAIEditorModule()
		: ModuleAssetCategoryBit(EAssetTypeCategories::Type::Misc)
	{

	}

	virtual void StartupModule() override
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.RegisterCustomPropertyTypeLayout(FStateKeyValueReference::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&StateKeyValueReferenceCustom::MakeInstance));

		PropertyModule.NotifyCustomizationModuleChanged();

		IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

		ModuleAssetCategoryBit = AssetTools.RegisterAdvancedAssetCategory(RpaiMenuCategoryKey, RpaiMenuCategoryText);

		ComposerBehaviorAssetTypeActions = MakeShared<FComposerBehaviorAssetTypeActions>(ModuleAssetCategoryBit);
		FAssetToolsModule::GetModule().Get().RegisterAssetTypeActions(ComposerBehaviorAssetTypeActions.ToSharedRef());
	}

	virtual void ShutdownModule() override
	{
		if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
		{
			FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
			PropertyModule.UnregisterCustomPropertyTypeLayout(FStateKeyValueReference::StaticStruct()->GetFName());
			PropertyModule.NotifyCustomizationModuleChanged();
		}

		if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
		{
			FAssetToolsModule::GetModule().Get().UnregisterAssetTypeActions(ComposerBehaviorAssetTypeActions.ToSharedRef());
		}
	}
};

IMPLEMENT_MODULE(FReasonablePlanningAIEditorModule, IReasonablePlanningAIEditorModule)

#undef LOCTEXT_NAMESPACE
