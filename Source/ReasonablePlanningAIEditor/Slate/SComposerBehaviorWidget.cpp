#include "SComposerBehaviorWidget.h"
#include "ISinglePropertyView.h"
#include "Editor.h"
#include "Widgets/SBoxPanel.h"
#include "UObject/Class.h"

#define LOCTEXT_NAMESPACE "ReasonablePlanningAIEditor"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SComposerBehaviorWidget::Construct(const FArguments& InArgs)
{
    ComposerBehavior = InArgs._ComposerBehavior;
    
    if(!ComposerBehavior.IsSet())
    {
        return;
    }
    
    FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
    
    FSinglePropertyParams PropertyParams;
    
    TSharedPtr<ISinglePropertyView> ConstructedStateTypePropertyView = PropertyEditorModule.CreateSingleProperty(ComposerBehavior.Get(), TEXT("ConstructedStateType"), PropertyParams);
    
    FDetailsViewArgs DetailsViewArgs;
    DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
    
    TSharedRef<IDetailsView> TestStartingStateDetailViewRef = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
    TSharedRef<IDetailsView> TestDesiredStateDetailViewRef = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
    
    TestStartingStateDetailView = TestStartingStateDetailViewRef;
    TestDesiredStateDetailView = TestDesiredStateDetailViewRef;
    
        OnStateTypePropertyChanged();
        ConstructedStateTypePropertyHandle = ConstructedStateTypePropertyView->GetPropertyHandle();
        ConstructedStateTypePropertyHandle->SetOnPropertyValueChanged(FSimpleDelegate::CreateSP(this, &SComposerBehaviorWidget::OnStateTypePropertyChanged));
        
        ChildSlot
        [
            SNew(SVerticalBox)
            +SVerticalBox::Slot()
            [
             SNew(STextBlock)
             .Text(LOCTEXT("ComposerWidgetBehavior_AssignClass", "Assign a type to ConstructedStateType to begin."))
            ]
            +SVerticalBox::Slot()
            [
             TestStartingStateDetailViewRef
            ]
            +SVerticalBox::Slot()
            [
             TestDesiredStateDetailViewRef
            ]
        ];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SComposerBehaviorWidget::AddReferencedObjects( FReferenceCollector& Collector )
{
    Collector.AddReferencedObject(TestStartingState);
    Collector.AddReferencedObject(TestDesiredState);
}

void SComposerBehaviorWidget::OnStateTypePropertyChanged()
{
    if(TestStartingState != nullptr && TestStartingState->IsValidLowLevel())
    {
        TestStartingState->ConditionalBeginDestroy();
    }
    
    if(TestDesiredState != nullptr && TestDesiredState->IsValidLowLevel())
    {
        TestDesiredState->ConditionalBeginDestroy();
    }
    
    TSubclassOf<URpaiState> ConstructedStateType = ComposerBehavior.Get()->GetConstructedStateType();
    if(ConstructedStateType.Get() != nullptr)
    {
        TestStartingState = NewObject<URpaiState>(GetTransientPackage(), ConstructedStateType);
        TestDesiredState = NewObject<URpaiState>(GetTransientPackage(), ConstructedStateType);
        
        TestStartingStateDetailView->SetObjects(TArray<UObject*>{ TestStartingState });
        TestDesiredStateDetailView->SetObjects(TArray<UObject*>{ TestDesiredState });
    }
}
