#include "SComposerBehaviorWidget.h"
#include "ISinglePropertyView.h"
#include "Editor.h"
#include "Widgets/SBoxPanel.h"
#include "UObject/Class.h"
#include "Core/RpaiPlannerBase.h"
#include "Core/RpaiReasonerBase.h"

#define LOCTEXT_NAMESPACE "ReasonablePlanningAIEditor"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SComposerBehaviorWidget::Construct(const FArguments& InArgs)
{
    ComposerBehavior = InArgs._ComposerBehavior;
    bIsExperimenting = false;
    LastPlanResult = ERpaiPlannerResult::Invalid;
    
    if(!ComposerBehavior.IsSet())
    {
        return;
    }
    
    FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
    
    FDetailsViewArgs DetailsViewArgs;
    DetailsViewArgs.bAllowSearch = false;
    DetailsViewArgs.bShowPropertyMatrixButton = false;
    DetailsViewArgs.bShowOptions = false;
    
    TSharedRef<IDetailsView> TestStartingStateDetailViewRef = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
    TestStartingStateDetailView = TestStartingStateDetailViewRef;
    
    NotifyStateTypePropertyChanged();
        
        ChildSlot
        [
            SNew(SVerticalBox)
            +SVerticalBox::Slot()
            [
             SNew(STextBlock)
             .Visibility(this, &SComposerBehaviorWidget::GetSetStateMessageVisibility)
             .Text(LOCTEXT("ComposerWidgetBehavior_AssignClass", "Assign a type to ConstructedStateType to begin."))
            ]
            +SVerticalBox::Slot()
            .AutoHeight()
            [
             SNew(SHorizontalBox)
             +SHorizontalBox::Slot()
             .AutoWidth()
             [
              SNew(SButton)
              .ToolTipText(LOCTEXT("ComposerWidgetBehavior_EvaluateTip", "Using the given input state below, run a full heuristics run on a Reasoner to determine a goal, and use that goal to determine actions from the Planner."))
              .Text(LOCTEXT("ComposerWidgetBehavior_Evaluate", "Evaluate Goal & Plan"))
              .OnClicked(this, &SComposerBehaviorWidget::OnEvaluateGoalAndPlan)
              .IsEnabled(this, &SComposerBehaviorWidget::IsEvaluateButtonEnabled)
             ]
            ]
            +SVerticalBox::Slot()
            [
             TestStartingStateDetailViewRef
            ]
        ];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SComposerBehaviorWidget::AddReferencedObjects( FReferenceCollector& Collector )
{
    Collector.AddReferencedObject(TestStartingState);
}

void SComposerBehaviorWidget::NotifyStateTypePropertyChanged()
{
    if(TestStartingState != nullptr && TestStartingState->IsValidLowLevel())
    {
        TestStartingState->ConditionalBeginDestroy();
    }
    
    TSubclassOf<URpaiState> ConstructedStateType = ComposerBehavior.Get()->GetConstructedStateType();
    if(ConstructedStateType.Get() != nullptr)
    {
        TestStartingState = NewObject<URpaiState>(GetTransientPackage(), ConstructedStateType);
        TestStartingStateDetailView->SetObject(TestStartingState, true);
    }
}

EVisibility SComposerBehaviorWidget::GetSetStateMessageVisibility() const
{
    TSubclassOf<URpaiState> ConstructedStateType = ComposerBehavior.Get()->GetConstructedStateType();
    return ConstructedStateType.Get() == nullptr ? EVisibility::Visible : EVisibility::Collapsed;
}

bool SComposerBehaviorWidget::IsEvaluateButtonEnabled() const
{
    return !bIsExperimenting && GetSetStateMessageVisibility() == EVisibility::Collapsed;
}

FReply SComposerBehaviorWidget::OnEvaluateGoalAndPlan()
{
    bIsExperimenting = true;
    return FReply::Handled();
}

void SComposerBehaviorWidget::Tick(const FGeometry & AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
    SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);
    if(bIsExperimenting)
    {
        TArray<URpaiActionBase*> PlannedActions;
        URpaiComposerBehavior* Behavior = ComposerBehavior.Get();
        const URpaiPlannerBase* Planner = Behavior->GetPlanner();
        const URpaiReasonerBase* Reasoner = Behavior->GetReasoner();
        switch(LastPlanResult)
        {
            case ERpaiPlannerResult::CompletedFailure:
            case ERpaiPlannerResult::CompletedCancelled:
                bIsExperimenting = false;
                LastPlanResult = ERpaiPlannerResult::Invalid;
                break;
            case ERpaiPlannerResult::CompletedSuccess:
                bIsExperimenting = false;
                LastPlanResult = ERpaiPlannerResult::Invalid;
                break;
            case ERpaiPlannerResult::RequiresTick:
                LastPlanResult = Planner->TickGoalPlanning(CurrentGoal, TestStartingState, Behavior->GetActions(), PlannedActions, CurrentPlannerMemory);
                break;
            case ERpaiPlannerResult::Invalid:
                CurrentGoal = Reasoner->ReasonNextGoal(Behavior->GetGoals(), TestStartingState);
                CurrentPlannerMemory = Planner->AllocateMemorySlice(ComponentActionMemory);
                LastPlanResult = Planner->StartGoalPlanning(CurrentGoal, TestStartingState, Behavior->GetActions(), PlannedActions, CurrentPlannerMemory);
                break;
            default:
                break;
        }
    }
}
