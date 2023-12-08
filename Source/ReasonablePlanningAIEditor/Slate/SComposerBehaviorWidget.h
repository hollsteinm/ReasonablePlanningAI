#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Composer/RpaiComposerBehavior.h"
#include "Core/RpaiGoalBase.h"

class SComposerBehaviorWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SComposerBehaviorWidget)
		: _ComposerBehavior(nullptr)
		{}
		SLATE_ATTRIBUTE(URpaiComposerBehavior*, ComposerBehavior)
	SLATE_END_ARGS()

	virtual void Construct(const FArguments& InArgs);
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime);
	virtual void AddReferencedObjects(FReferenceCollector& Collector);

	void NotifyStateTypePropertyChanged();
	void NotifyGoalsPropertyChanged();

private:
	// Slate bound
	TAttribute<URpaiComposerBehavior*> ComposerBehavior;

	// Internal state
	URpaiState* TestStartingState;
	TSharedPtr<IDetailsView> TestStartingStateDetailView;

	// Evaluation state management
	bool bIsExperimenting;
	ERpaiPlannerResult LastPlanResult;
	FRpaiMemoryStruct CurrentPlannerMemory;
	FRpaiMemory ComponentActionMemory;
	URpaiGoalBase* CurrentGoal;

	// UI elements and callbacks
	EVisibility GetSetStateMessageVisibility() const;
	EVisibility GetSummaryVisibility() const;
	bool IsEvaluateButtonEnabled() const;
	bool IsEvaluateGoalButtonEnabled() const;
	FReply OnEvaluateGoalAndPlan();
	FReply OnEvaluatePlanWithGoal();
	TSharedRef<SWidget> GoalSelectionContent();
	TSharedRef<SWidget> OnGenerateGoalRow(URpaiGoalBase* Item);
	TSharedRef<ITableRow> OnGenerateExperimentOutputRow(TSharedPtr<FText> InText, const TSharedRef<STableViewBase>& OwnerTable);
	FText GetCurrentGoalSelectionText() const;
	void HandleGoalSelectionChanged(URpaiGoalBase* Selection, ESelectInfo::Type SelectInfo);
	void EmitPlanOutput(const URpaiGoalBase* Goal, const TArray<URpaiActionBase*>& Actions);

	// Assigned Slate objects
	TSharedPtr<SComboBox<URpaiGoalBase*>> GoalComboBox;
	TSharedPtr<SListView<TSharedPtr<FText>>> ExperimentOutputList;

	// Internal Copies for Slate components
	TArray<URpaiGoalBase*> Goals;
	TArray<TSharedPtr<FText>> ExperimentOutput;
};
