// Copyright Troll Purse. All Rights Reserved.

#include "ReasonablePlanningAIEditor.h"
#include "StateKeyValueReferenceCustom.h"
#include "Core/RpaiTypes.h"

#define LOCTEXT_NAMESPACE "ResonablePlanningAIEditor"

struct FReasonablePlanningAIEditorModule : public IReasonablePlanningAIEditorModule
{
	virtual void StartupModule() override
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.RegisterCustomPropertyTypeLayout(FStateKeyValueReference::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&StateKeyValueReferenceCustom::MakeInstance));

		PropertyModule.NotifyCustomizationModuleChanged();
	}

	virtual void ShutdownModule() override
	{
		if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
		{
			FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
			PropertyModule.UnregisterCustomPropertyTypeLayout(FStateKeyValueReference::StaticStruct()->GetFName());
			PropertyModule.NotifyCustomizationModuleChanged();
		}
	}
};

IMPLEMENT_MODULE(FReasonablePlanningAIEditorModule, IReasonablePlanningAIEditorModule)

#undef LOCTEXT_NAMESPACE