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
    NotifyGoalsPropertyChanged();
        
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
              SNew(STextBlock)
              .Visibility(this, &SComposerBehaviorWidget::GetSetStateMessageVisibility)
              .Text(LOCTEXT("ComposerWidgetBehavior_Summary", "Modify the state details below to run experiments by using the available button actions."))
            ]
            +SVerticalBox::Slot()
            .AutoHeight()
            [
             SNew(SHorizontalBox)
             +SHorizontalBox::Slot()
             .AutoWidth()
             [
              SNew(SButton)
              .ToolTipText(LOCTEXT("ComposerWidgetBehavior_FullEvaluateTip", "Using the given input state below, run a full heuristics run on a Reasoner to determine a goal, and use that goal to determine actions from the Planner."))
              .Text(LOCTEXT("ComposerWidgetBehavior_FullEvaluate", "Evaluate Goal & Plan"))
              .OnClicked(this, &SComposerBehaviorWidget::OnEvaluateGoalAndPlan)
              .IsEnabled(this, &SComposerBehaviorWidget::IsEvaluateButtonEnabled)
             ]
             +SHorizontalBox::Slot()
             .AutoWidth()
             [
               GoalSelectionContent()
             ]
             +SHorizontalBox::Slot()
             .AutoWidth()
             [
              SNew(SButton)
              .ToolTipText(LOCTEXT("ComposerWidgetBehavior_ActionsEvaluateTip", "Using the given input state below and a selected goal from the state, determine actions from the Planner."))
              .Text(LOCTEXT("ComposerWidgetBehavior_ActionsEvaluate", "Evaluate Plan with Goal"))
              .OnClicked(this, &SComposerBehaviorWidget::OnEvaluatePlanWithGoal)
              .IsEnabled(this, &SComposerBehaviorWidget::IsEvaluateGoalButtonEnabled)
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

void SComposerBehaviorWidget::NotifyGoalsPropertyChanged()
{
    Goals = ComposerBehavior.Get()->GetGoals();
    CurrentGoal = nullptr;
    if(GoalComboBox)
    {
        GoalComboBox->RefreshOptions();
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

EVisibility SComposerBehaviorWidget::GetSummaryVisibility() const
{
    return GetSetStateMessageVisibility() == EVisibility::Collapsed ? EVisibility::Visible : EVisibility::Collapsed;
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
                // Emit Actions
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

bool SComposerBehaviorWidget::IsEvaluateGoalButtonEnabled() const
{
    return IsEvaluateButtonEnabled() && CurrentGoal != nullptr && CurrentGoal->IsValidLowLevel();
}

FReply SComposerBehaviorWidget::OnEvaluatePlanWithGoal()
{
    TArray<URpaiActionBase*> PlannedActions;
    URpaiComposerBehavior* Behavior = ComposerBehavior.Get();
    const URpaiPlannerBase* Planner = Behavior->GetPlanner();
    CurrentPlannerMemory = Planner->AllocateMemorySlice(ComponentActionMemory);
    LastPlanResult = Planner->StartGoalPlanning(CurrentGoal, TestStartingState, Behavior->GetActions(), PlannedActions, CurrentPlannerMemory);
    if(LastPlanResult == ERpaiPlannerResult::RequiresTick)
    {
        bIsExperimenting = true;
    }
    else
    {
        // Emit Actions
    }

    return FReply::Handled();
}

TSharedRef<SWidget> SComposerBehaviorWidget::GoalSelectionContent()
{
    return SAssignNew(GoalComboBox, SComboBox<URpaiGoalBase*>)
            .OptionsSource(&Goals)
            .OnSelectionChanged(this, &SComposerBehaviorWidget::HandleGoalSelectionChanged)
            .OnGenerateWidget(this, &SComposerBehaviorWidget::OnGenerateGoalRow)
            .InitiallySelectedItem(CurrentGoal)
            [
                SNew(STextBlock)
                .Text(this, &SComposerBehaviorWidget::GetCurrentGoalSelectionText)
            ];
}

TSharedRef<SWidget> SComposerBehaviorWidget::OnGenerateGoalRow(URpaiGoalBase* Item)
{
    if(Item != nullptr && Item->IsValidLowLevel())
    {
        return SNew(STextBlock)
            .Text(FText::FromString(Item->GetGoalName()));
    }
    return SNew(STextBlock)
        .Text(LOCTEXT("ComposerWidgetBehavior_InvalidObject", "Invalid Object!"));
}

FText SComposerBehaviorWidget::GetCurrentGoalSelectionText() const
{
    if(CurrentGoal != nullptr && CurrentGoal->IsValidLowLevel())
    {
        return FText::FromString(CurrentGoal->GetGoalName());
    }
    return LOCTEXT("ComposerWidgetBehavior_SelectGoalPrompt", "Select a Goal");
}

void SComposerBehaviorWidget::HandleGoalSelectionChanged(URpaiGoalBase* Selection, ESelectInfo::Type SelectInfo)
{
    if(SelectInfo != ESelectInfo::OnNavigation)
    {
        CurrentGoal = Selection;
        GoalComboBox->SetSelectedItem(CurrentGoal);
    }
}
