// Copyright Troll Purse. All Rights Reserved.

#include "ReasonablePlanningAIEditor.h"
#include "StateKeyValueReferenceCustom.h"
#include "StateQueryComparisonCustom.h"
#include "StateTypePropertyMultiBindCustom.h"
#include "ComposerBehaviorCustom.h"
#include "Core/RpaiTypes.h"
#include "Composer/RpaiComposerBehavior.h"
#include "Composer/Queries/RpaiStateQuery_CompareToBool.h"
#include "Composer/Queries/RpaiStateQuery_CompareToDistance.h"
#include "Composer/Queries/RpaiStateQuery_CompareToFloat.h"
#include "Composer/Queries/RpaiStateQuery_CompareToInteger.h"
#include "Composer/Queries/RpaiStateQuery_CompareToStateValue.h"
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
		ComposerStateQueryCustom::AddChildCustomization(URpaiStateQuery_CompareToBool::StaticClass()->GetFName(), StateQueryComparisonCustom::MakePropertyInstance(TEXT("bValueToCompare")));
		ComposerStateQueryCustom::AddChildCustomization(URpaiStateQuery_CompareToDistance::StaticClass()->GetFName(), StateQueryComparisonCustom::MakePropertyInstance(TEXT("Distance")));
		ComposerStateQueryCustom::AddChildCustomization(URpaiStateQuery_CompareToFloat::StaticClass()->GetFName(), StateQueryComparisonCustom::MakePropertyInstance(TEXT("ValueToCompare")));
		ComposerStateQueryCustom::AddChildCustomization(URpaiStateQuery_CompareToInteger::StaticClass()->GetFName(), StateQueryComparisonCustom::MakePropertyInstance(TEXT("ValueToCompare")));
		ComposerStateQueryCustom::AddChildCustomization(URpaiStateQuery_CompareToStateValue::StaticClass()->GetFName(), StateQueryComparisonCustom::MakePropertyInstance(TEXT("ComparedToState")));

		FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.RegisterCustomPropertyTypeLayout(FStateKeyValueReference::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&StateKeyValueReferenceCustom::MakeInstance));
		PropertyModule.RegisterCustomPropertyTypeLayout(FRpaiStateTypePropertyMultiBind::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&StateTypePropertyMultiBindCustom::MakeInstance));
		PropertyModule.RegisterCustomPropertyTypeLayout(URpaiStateQuery_CompareToBool::StaticClass()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&StateQueryComparisonCustom::MakePropertyInstance, FName(TEXT("bValueToCompare"))));
		PropertyModule.RegisterCustomPropertyTypeLayout(URpaiStateQuery_CompareToDistance::StaticClass()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&StateQueryComparisonCustom::MakePropertyInstance, FName(TEXT("Distance"))));
		PropertyModule.RegisterCustomPropertyTypeLayout(URpaiStateQuery_CompareToFloat::StaticClass()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&StateQueryComparisonCustom::MakePropertyInstance, FName(TEXT("ValueToCompare"))));
		PropertyModule.RegisterCustomPropertyTypeLayout(URpaiStateQuery_CompareToInteger::StaticClass()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&StateQueryComparisonCustom::MakePropertyInstance, FName(TEXT("ValueToCompare"))));
		PropertyModule.RegisterCustomPropertyTypeLayout(URpaiStateQuery_CompareToStateValue::StaticClass()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&StateQueryComparisonCustom::MakePropertyInstance, FName(TEXT("ComparedToState"))));
		PropertyModule.RegisterCustomPropertyTypeLayout(URpaiComposerStateQuery::StaticClass()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&ComposerStateQueryCustom::MakeInstance));

		PropertyModule.RegisterCustomClassLayout(URpaiStateQuery_CompareToBool::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&StateQueryComparisonCustom::MakeInstance, FName(TEXT("bValueToCompare"))));
		PropertyModule.RegisterCustomClassLayout(URpaiStateQuery_CompareToDistance::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&StateQueryComparisonCustom::MakeInstance, FName(TEXT("Distance"))));
		PropertyModule.RegisterCustomClassLayout(URpaiStateQuery_CompareToFloat::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&StateQueryComparisonCustom::MakeInstance, FName(TEXT("ValueToCompare"))));
		PropertyModule.RegisterCustomClassLayout(URpaiStateQuery_CompareToInteger::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&StateQueryComparisonCustom::MakeInstance, FName(TEXT("ValueToCompare"))));
		PropertyModule.RegisterCustomClassLayout(URpaiStateQuery_CompareToStateValue::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&StateQueryComparisonCustom::MakeInstance, FName(TEXT("ComparedToState"))));
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
			PropertyModule.UnregisterCustomClassLayout(URpaiStateQuery_CompareToDistance::StaticClass()->GetFName());
			PropertyModule.UnregisterCustomClassLayout(URpaiStateQuery_CompareToFloat::StaticClass()->GetFName());
			PropertyModule.UnregisterCustomClassLayout(URpaiStateQuery_CompareToInteger::StaticClass()->GetFName());
			PropertyModule.UnregisterCustomClassLayout(URpaiStateQuery_CompareToStateValue::StaticClass()->GetFName());
			PropertyModule.UnregisterCustomClassLayout(URpaiComposerStateQuery::StaticClass()->GetFName());

			PropertyModule.UnregisterCustomPropertyTypeLayout(URpaiStateQuery_CompareToBool::StaticClass()->GetFName());
			PropertyModule.UnregisterCustomPropertyTypeLayout(URpaiStateQuery_CompareToDistance::StaticClass()->GetFName());
			PropertyModule.UnregisterCustomPropertyTypeLayout(URpaiStateQuery_CompareToFloat::StaticClass()->GetFName());
			PropertyModule.UnregisterCustomPropertyTypeLayout(URpaiStateQuery_CompareToInteger::StaticClass()->GetFName());
			PropertyModule.UnregisterCustomPropertyTypeLayout(URpaiStateQuery_CompareToStateValue::StaticClass()->GetFName());
			PropertyModule.UnregisterCustomPropertyTypeLayout(FStateKeyValueReference::StaticStruct()->GetFName());
			PropertyModule.UnregisterCustomPropertyTypeLayout(FRpaiStateTypePropertyMultiBind::StaticStruct()->GetFName());
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
