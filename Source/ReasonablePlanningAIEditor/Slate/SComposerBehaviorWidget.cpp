#include "SComposerBehaviorWidget.h"
#include "ISinglePropertyView.h"
#include "Editor.h"
#include "Widgets/SBoxPanel.h"
#include "SlateCore.h"
#include "UObject/Class.h"
#include "Core/RpaiPlannerBase.h"
#include "Core/RpaiReasonerBase.h"
#include "Core/RpaiGoalBase.h"
#include "Core/RpaiActionBase.h"

#define LOCTEXT_NAMESPACE "ReasonablePlanningAIEditor"

// structure for debug views of data tables
struct FRpaiDiagnosticsViewData
{
	TArray<FText> ArbitraryRowData;
};

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SComposerBehaviorWidget::Construct(const FArguments& InArgs)
{
	ComposerBehavior = InArgs._ComposerBehavior;
	bIsExperimenting = false;
	LastPlanResult = ERpaiPlannerResult::Invalid;

	if (!ComposerBehavior.IsSet())
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
				+ SVerticalBox::Slot()
				[
					SNew(STextBlock)
						.Visibility(this, &SComposerBehaviorWidget::GetSetStateMessageVisibility)
						.Text(LOCTEXT("ComposerWidgetBehavior_AssignClass", "Assign a type to ConstructedStateType to begin."))
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(STextBlock)
						.Visibility(this, &SComposerBehaviorWidget::GetSetStateMessageVisibility)
						.Text(LOCTEXT("ComposerWidgetBehavior_Summary", "Modify the state details below to run experiments by using the available button actions."))
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							SNew(SButton)
								.ToolTipText(LOCTEXT("ComposerWidgetBehavior_FullEvaluateTip", "Using the given input state below, preview how actions will be evaluated. Additionally shows the goal weights and distances after the action is performed."))
								.Text(LOCTEXT("ComposerWidgetBehavior_ActionOnlyEvaluate", "Evaluate Actions"))
								.OnClicked(this, &SComposerBehaviorWidget::OnEvaluateActions)
								.IsEnabled(this, &SComposerBehaviorWidget::IsEvaluateButtonEnabled)
						]
						+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							SNew(SButton)
								.ToolTipText(LOCTEXT("ComposerWidgetBehavior_FullEvaluateTip", "Using the given input state below, preview how goals will be evaluated."))
								.Text(LOCTEXT("ComposerWidgetBehavior_GoalOnlyEvaluate", "Evaluate Goals"))
								.OnClicked(this, &SComposerBehaviorWidget::OnEvaluateGoals)
								.IsEnabled(this, &SComposerBehaviorWidget::IsEvaluateButtonEnabled)
						]
						+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							SNew(SButton)
								.ToolTipText(LOCTEXT("ComposerWidgetBehavior_FullEvaluateTip", "Using the given input state below, run a full heuristics run on a Reasoner to determine a goal, and use that goal to determine actions from the Planner."))
								.Text(LOCTEXT("ComposerWidgetBehavior_FullEvaluate", "Evaluate Goal & Plan"))
								.OnClicked(this, &SComposerBehaviorWidget::OnEvaluateGoalAndPlan)
								.IsEnabled(this, &SComposerBehaviorWidget::IsEvaluateButtonEnabled)
						]
						+ SHorizontalBox::Slot()
						.FillWidth(1.f)
						.HAlign(EHorizontalAlignment::HAlign_Right)
						[
							SNew(SHorizontalBox)
								+ SHorizontalBox::Slot()
								.AutoWidth()
								[
									GoalSelectionContent()
								]
								+ SHorizontalBox::Slot()
								.AutoWidth()
								[
									SNew(SButton)
										.ToolTipText(LOCTEXT("ComposerWidgetBehavior_ActionsEvaluateTip", "Using the given input state below and a selected goal from the state, determine actions from the Planner."))
										.Text(LOCTEXT("ComposerWidgetBehavior_SetGoalActionsEvaluate", "Evaluate Plan with Goal"))
										.OnClicked(this, &SComposerBehaviorWidget::OnEvaluatePlanWithGoal)
										.IsEnabled(this, &SComposerBehaviorWidget::IsEvaluateGoalButtonEnabled)
								]
						]
				]
				+ SVerticalBox::Slot()
				[
					TestStartingStateDetailViewRef
				]
				+ SVerticalBox::Slot()
				[
					SNew(SBorder)
						.Content()
						[
							SNew(SScrollBox)
								+ SScrollBox::Slot()
								[
									SAssignNew(ExperimentOutputList, SListView<TSharedPtr<FRpaiDiagnosticsViewData>>)
										.ItemHeight(24)
										.SelectionMode(ESelectionMode::None)
										.ListItemsSource(&ExperimentOutput)
										.OnGenerateRow(this, &SComposerBehaviorWidget::OnGenerateExperimentOutputRow)

								]
						]
				]
		];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SComposerBehaviorWidget::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObject(TestStartingState);
}

void SComposerBehaviorWidget::NotifyStateTypePropertyChanged()
{
	if (TestStartingState != nullptr && TestStartingState->IsValidLowLevel())
	{
		TestStartingState->ConditionalBeginDestroy();
	}

	TSubclassOf<URpaiState> ConstructedStateType = ComposerBehavior.Get()->GetConstructedStateType();
	if (ConstructedStateType.Get() != nullptr)
	{
		TestStartingState = NewObject<URpaiState>(GetTransientPackage(), ConstructedStateType);
		TestStartingStateDetailView->SetObject(TestStartingState, true);
	}
}

void SComposerBehaviorWidget::NotifyGoalsPropertyChanged()
{
	Goals = ComposerBehavior.Get()->GetGoals();
	CurrentGoal = nullptr;
	if (GoalComboBox)
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

void SComposerBehaviorWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);
	if (bIsExperimenting)
	{
		TArray<URpaiActionBase*> PlannedActions;
		URpaiComposerBehavior* Behavior = ComposerBehavior.Get();
		const URpaiPlannerBase* Planner = Behavior->GetPlanner();
		const URpaiReasonerBase* Reasoner = Behavior->GetReasoner();
		switch (LastPlanResult)
		{
		case ERpaiPlannerResult::CompletedFailure:
		case ERpaiPlannerResult::CompletedCancelled:
			bIsExperimenting = false;
			LastPlanResult = ERpaiPlannerResult::Invalid;
			break;
		case ERpaiPlannerResult::CompletedSuccess:
			bIsExperimenting = false;
			LastPlanResult = ERpaiPlannerResult::Invalid;
			EmitPlanOutput(CurrentGoal, PlannedActions);
			break;
		case ERpaiPlannerResult::RequiresTick:
			LastPlanResult = Planner->TickGoalPlanning(CurrentGoal, TestStartingState, Behavior->GetActions(), PlannedActions, CurrentPlannerMemory);
			if (LastPlanResult == ERpaiPlannerResult::CompletedSuccess)
			{
				bIsExperimenting = false;
				LastPlanResult = ERpaiPlannerResult::Invalid;
				EmitPlanOutput(CurrentGoal, PlannedActions);
			}
			break;
		case ERpaiPlannerResult::Invalid:
			CurrentGoal = Reasoner->ReasonNextGoal(Behavior->GetGoals(), TestStartingState);
			GoalComboBox->SetSelectedItem(CurrentGoal);
			CurrentPlannerMemory = Planner->AllocateMemorySlice(ComponentActionMemory);
			LastPlanResult = Planner->StartGoalPlanning(CurrentGoal, TestStartingState, Behavior->GetActions(), PlannedActions, CurrentPlannerMemory);
			if (LastPlanResult == ERpaiPlannerResult::CompletedSuccess)
			{
				bIsExperimenting = false;
				LastPlanResult = ERpaiPlannerResult::Invalid;
				EmitPlanOutput(CurrentGoal, PlannedActions);
			}
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
	if (LastPlanResult == ERpaiPlannerResult::RequiresTick)
	{
		bIsExperimenting = true;
	}
	else
	{
		EmitPlanOutput(CurrentGoal, PlannedActions);
	}

	return FReply::Handled();
}

FReply SComposerBehaviorWidget::OnEvaluateActions()
{
	EmitCurrentStateActionWeightsAndDistances();
	return FReply::Handled();
}

FReply SComposerBehaviorWidget::OnEvaluateGoals()
{
	EmitCurrentStateGoalCategoryWeightDistance();
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
	if (Item != nullptr && Item->IsValidLowLevel())
	{
		return SNew(STextBlock)
			.Text(FText::FromString(Item->GetGoalName()));
	}
	return SNew(STextBlock)
		.Text(LOCTEXT("ComposerWidgetBehavior_InvalidObject", "Invalid Object!"));
}

FText SComposerBehaviorWidget::GetCurrentGoalSelectionText() const
{
	if (CurrentGoal != nullptr && CurrentGoal->IsValidLowLevel())
	{
		return FText::FromString(CurrentGoal->GetGoalName());
	}
	return LOCTEXT("ComposerWidgetBehavior_SelectGoalPrompt", "Select a Goal");
}

void SComposerBehaviorWidget::HandleGoalSelectionChanged(URpaiGoalBase* Selection, ESelectInfo::Type SelectInfo)
{
	if (SelectInfo != ESelectInfo::OnNavigation)
	{
		CurrentGoal = Selection;
		GoalComboBox->SetSelectedItem(CurrentGoal);
	}
}

void SComposerBehaviorWidget::EmitPlanOutput(const URpaiGoalBase* Goal, const TArray<URpaiActionBase*>& Actions)
{
	ExperimentOutput.Empty();

	TSharedPtr<FRpaiDiagnosticsViewData> GoalNameRow = MakeShareable(new FRpaiDiagnosticsViewData());
	GoalNameRow->ArbitraryRowData.Add(LOCTEXT("ComposerWidgetBehavior_GoalOutput_GoalNameRow_Title", "Goal Name"));
	GoalNameRow->ArbitraryRowData.Add(FText::FromString(Goal->GetGoalName()));
	ExperimentOutput.Add(GoalNameRow);

	TSharedPtr<FRpaiDiagnosticsViewData> GoalCategoryRow = MakeShareable(new FRpaiDiagnosticsViewData());
	GoalCategoryRow->ArbitraryRowData.Add(LOCTEXT("ComposerWidgetBehavior_GoalOutput_GoalCategoryRow_Title", "Goal Category"));
	GoalCategoryRow->ArbitraryRowData.Add(FText::Format(LOCTEXT("ComposerWidgetBehavior_GoalOutput_Category", "{0}"), Goal->GetCategory()));
	ExperimentOutput.Add(GoalCategoryRow);

	TSharedPtr<FRpaiDiagnosticsViewData> GoalWeightRow = MakeShareable(new FRpaiDiagnosticsViewData());
	GoalWeightRow->ArbitraryRowData.Add(LOCTEXT("ComposerWidgetBehavior_GoalOutput_GoalWeightRow_Title", "Goal Weight"));
	GoalWeightRow->ArbitraryRowData.Add(FText::Format(LOCTEXT("ComposerWidgetBehavior_GoalOutput_Weight", "{0}"), Goal->GetWeight(TestStartingState)));
	ExperimentOutput.Add(GoalWeightRow);

	for (const auto& Action : Actions)
	{
		TSharedPtr<FRpaiDiagnosticsViewData> ActionRow = MakeShareable(new FRpaiDiagnosticsViewData());
		ActionRow->ArbitraryRowData.Add(LOCTEXT("ComposerWidgetBehavior_GoalOutput_GoalWeightRow_Title", "Action Name"));
		ActionRow->ArbitraryRowData.Add(FText::Format(LOCTEXT("ComposerWidgetBehavior_ActionOutput", "{0}"), FText::FromString(Action->GetActionName())));
		ExperimentOutput.Add(ActionRow);
	}
	ExperimentOutputList->RequestListRefresh();
}

void SComposerBehaviorWidget::EmitCurrentStateActionWeightsAndDistances()
{
	if (ComposerBehavior.IsSet())
	{
		URpaiComposerBehavior* Behavior = ComposerBehavior.Get();
		if (Behavior == nullptr)
		{
			return;
		}

		ExperimentOutput.Empty();
		TArray<URpaiActionBase*> BehaviorActions = Behavior->GetActions();
		TArray<URpaiGoalBase*> BehaviorGoals = Behavior->GetGoals();

		TSharedPtr<FRpaiDiagnosticsViewData> DynamicHeaderRow = MakeShareable(new FRpaiDiagnosticsViewData());
		DynamicHeaderRow->ArbitraryRowData.Add(LOCTEXT("ComposerWidgetBehavior_ActionPreviewOutput_ActionNameHeader", "Action Name"));
		DynamicHeaderRow->ArbitraryRowData.Add(LOCTEXT("ComposerWidgetBehavior_ActionPreviewOutput_ActionWeightHeader", "Action Weight"));
		DynamicHeaderRow->ArbitraryRowData.Add(LOCTEXT("ComposerWidgetBehavior_ActionPreviewOutput_ActionApplicableHeader", "Action Is Applicable?"));
		for (const auto& Goal : BehaviorGoals)
		{
			FText GoalNameText = FText::FromString(Goal->GetGoalName());
			DynamicHeaderRow->ArbitraryRowData.Add(FText::Format(LOCTEXT("ComposerWidgetBehavior_ActionPreviewOutput_GoalCategoryHeader_Fmt", "{0}.Category"), GoalNameText));
			DynamicHeaderRow->ArbitraryRowData.Add(FText::Format(LOCTEXT("ComposerWidgetBehavior_ActionPreviewOutput_GoalWeightHeader_Fmt", "{0}.Weight"), GoalNameText));
			DynamicHeaderRow->ArbitraryRowData.Add(FText::Format(LOCTEXT("ComposerWidgetBehavior_ActionPreviewOutput_GoalDistanceHeader_Fmt", "{0}.Distance"), GoalNameText));
		}
		ExperimentOutput.Add(DynamicHeaderRow);

		for (const auto& Action : BehaviorActions)
		{
			URpaiState* FutureState = NewObject<URpaiState>(GetTransientPackage(), TestStartingState->GetClass());
			Action->ApplyToState(FutureState);

			TSharedPtr<FRpaiDiagnosticsViewData> ActionRow = MakeShareable(new FRpaiDiagnosticsViewData());

			ActionRow->ArbitraryRowData.Add(FText::FromString(Action->GetActionName()));
			ActionRow->ArbitraryRowData.Add(FText::Format(LOCTEXT("ComposerWidgetBehavior_ActionPreviewOutput_ActionWeightValue_Fmt", "{0}"), Action->ExecutionWeight(TestStartingState)));
			ActionRow->ArbitraryRowData.Add(FText::Format(LOCTEXT("ComposerWidgetBehavior_ActionPreviewOutput_ActionApplicableValue_Fmt", "{0}"), Action->IsApplicable(TestStartingState)));

			for (const auto& Goal : BehaviorGoals)
			{
				ActionRow->ArbitraryRowData.Add(FText::Format(LOCTEXT("ComposerWidgetBehavior_ActionPreviewOutput_GoalCategoryValue_Fmt", "{0}"), Goal->GetCategory()));
				ActionRow->ArbitraryRowData.Add(FText::Format(LOCTEXT("ComposerWidgetBehavior_ActionPreviewOutput_GoalWeightValue_Fmt", "{0}"), Goal->GetWeight(FutureState)));
				ActionRow->ArbitraryRowData.Add(FText::Format(LOCTEXT("ComposerWidgetBehavior_ActionPreviewOutput_GoalDistanceValue_Fmt", "{0}"), Goal->GetDistanceToDesiredState(FutureState)));
			}
			ExperimentOutput.Add(ActionRow);
		}
		ExperimentOutputList->RequestListRefresh();
	}
}

void SComposerBehaviorWidget::EmitCurrentStateGoalCategoryWeightDistance()
{
	if (ComposerBehavior.IsSet())
	{
		URpaiComposerBehavior* Behavior = ComposerBehavior.Get();
		if (Behavior == nullptr)
		{
			return;
		}
		TArray<URpaiGoalBase*> BehaviorGoals = Behavior->GetGoals();

		ExperimentOutput.Empty();

		TSharedPtr<FRpaiDiagnosticsViewData> GoalHeaderRow = MakeShareable(new FRpaiDiagnosticsViewData());
		GoalHeaderRow->ArbitraryRowData.Add(LOCTEXT("ComposerWidgetBehavior_GoalOutput_GoalNameRow_Title", "Goal Name"));
		GoalHeaderRow->ArbitraryRowData.Add(LOCTEXT("ComposerWidgetBehavior_GoalOutput_GoalCategoryRow_Title", "Goal Category"));
		GoalHeaderRow->ArbitraryRowData.Add(LOCTEXT("ComposerWidgetBehavior_GoalOutput_GoalWeightRow_Title", "Goal Weight"));
		ExperimentOutput.Add(GoalHeaderRow);

		for (const auto& Goal : BehaviorGoals)
		{
			TSharedPtr<FRpaiDiagnosticsViewData> GoalRow = MakeShareable(new FRpaiDiagnosticsViewData());
			GoalRow->ArbitraryRowData.Add(FText::FromString(Goal->GetGoalName()));
			GoalRow->ArbitraryRowData.Add(FText::Format(LOCTEXT("ComposerWidgetBehavior_GoalOutput_Category", "{0}"), Goal->GetCategory()));
			GoalRow->ArbitraryRowData.Add(FText::Format(LOCTEXT("ComposerWidgetBehavior_GoalOutput_Weight", "{0}"), Goal->GetWeight(TestStartingState)));
			ExperimentOutput.Add(GoalRow);
		}
		ExperimentOutputList->RequestListRefresh();
	}
}

TSharedRef<ITableRow> SComposerBehaviorWidget::OnGenerateExperimentOutputRow(TSharedPtr<FRpaiDiagnosticsViewData> RowData, const TSharedRef<STableViewBase>& OwnerTable)
{
	TSharedRef<SHorizontalBox> Cells = SNew(SHorizontalBox);
	for (const auto& Text : RowData->ArbitraryRowData)
	{
		Cells->AddSlot()
			[
				SNew(SBorder)
					.Padding(0.5f, 0.5f)
					[
						SNew(STextBlock)
							.Margin(FMargin(3.f, 3.f))
							.Text(Text)
					]
			];
	}
	return SNew(STableRow<TSharedPtr<FRpaiDiagnosticsViewData>>, OwnerTable)
		[
			Cells
		];
}
