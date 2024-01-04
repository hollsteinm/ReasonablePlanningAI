// Copyright Troll Purse. All Rights Reserved.

#include "ReasonablePlanningAIEditor.h"
#include "StateKeyValueReferenceCustom.h"
#include "StateQueryComparisonCustom.h"
#include "ComposerBehaviorCustom.h"
#include "Core/RpaiTypes.h"
#include "Composer/RpaiComposerBehavior.h"
#include "Composer/Queries/RpaiStateQuery_CompareToBool.h"
#include "ComposerBehaviorAssetTypeActions.h"
#include "ComposerStateQueryCustom.h"

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
		ComposerStateQueryCustom::AddChildCustomization(URpaiStateQuery_CompareTo::StaticClass()->GetFName(), StateQueryComparisonCustom::MakePropertyInstance(NAME_None));
		ComposerStateQueryCustom::AddChildCustomization(URpaiStateQuery_CompareToBool::StaticClass()->GetFName(), StateQueryComparisonCustom::MakePropertyInstance(TEXT("bValueToCompare")));

		FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.RegisterCustomPropertyTypeLayout(FStateKeyValueReference::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&StateKeyValueReferenceCustom::MakeInstance));
		PropertyModule.RegisterCustomPropertyTypeLayout(URpaiStateQuery_CompareTo::StaticClass()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&StateQueryComparisonCustom::MakePropertyInstance, FName(NAME_None)));
		PropertyModule.RegisterCustomPropertyTypeLayout(URpaiStateQuery_CompareToBool::StaticClass()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&StateQueryComparisonCustom::MakePropertyInstance, FName(TEXT("bValueToCompare"))));
		PropertyModule.RegisterCustomPropertyTypeLayout(URpaiComposerStateQuery::StaticClass()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&ComposerStateQueryCustom::MakeInstance));

		PropertyModule.RegisterCustomClassLayout(URpaiStateQuery_CompareTo::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&StateQueryComparisonCustom::MakeInstance, FName(NAME_None)));
		PropertyModule.RegisterCustomClassLayout(URpaiStateQuery_CompareToBool::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&StateQueryComparisonCustom::MakeInstance, FName(TEXT("bValueToCompare"))));
		PropertyModule.RegisterCustomClassLayout(URpaiComposerBehavior::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&ComposerBehaviorCustom::MakeInstance));
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
			PropertyModule.UnregisterCustomClassLayout(URpaiComposerBehavior::StaticClass()->GetFName());
			PropertyModule.UnregisterCustomClassLayout(URpaiStateQuery_CompareToBool::StaticClass()->GetFName());
			PropertyModule.UnregisterCustomClassLayout(URpaiStateQuery_CompareTo::StaticClass()->GetFName());
			PropertyModule.UnregisterCustomClassLayout(URpaiComposerStateQuery::StaticClass()->GetFName());

			PropertyModule.UnregisterCustomPropertyTypeLayout(URpaiStateQuery_CompareToBool::StaticClass()->GetFName());
			PropertyModule.UnregisterCustomPropertyTypeLayout(URpaiStateQuery_CompareTo::StaticClass()->GetFName());
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
