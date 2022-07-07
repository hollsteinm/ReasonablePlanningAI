// Troll Purse. All rights reserved.


#include "Composer/RPAIComposerBrainComponent.h"
#include "Composer/ReasonablePlanningAIBehavior.h"
#include "Core/ReasonablePlanningTypes.h"
#include "Core/ReasonablePlanningActionBase.h"
#include "Core/ReasonablePlanningGoalBase.h"
#include "Core/ReasonablePlanningReasonerBase.h"
#include "Core/ReasonablePlanningPlannerBase.h"
#include "VisualLogger/VisualLoggerTypes.h"
#include "VisualLogger/VisualLogger.h"
#include "AIController.h"

URPAIComposerBrainComponent::URPAIComposerBrainComponent()
	: ReasonablePlanningBehavior(nullptr)
	, CurrentAction(nullptr)
	, PlannedActions({})
	, CurrentGoal(nullptr)
	, CurrentState(nullptr)
{
}

void URPAIComposerBrainComponent::UnregisterOldAction(UReasonablePlanningActionBase* OldAction)
{
	if (OldAction == nullptr)
	{
		return;
	}

	OldAction->OnActionStarted.RemoveAll(this);
	OldAction->OnActionCancelled.RemoveAll(this);
}

void URPAIComposerBrainComponent::RegisterNewAction(UReasonablePlanningActionBase* NewAction)
{
	if (NewAction == nullptr)
	{
		return;
	}

	NewAction->OnActionComplete.AddUniqueDynamic(this, &URPAIComposerBrainComponent::OnActionCompleted);
	NewAction->OnActionCancelled.AddUniqueDynamic(this, &URPAIComposerBrainComponent::OnActionCancelled);
}

void URPAIComposerBrainComponent::PopNextAction()
{
	if (PlannedActions.Num() > 0)
	{
		UnregisterOldAction(CurrentAction);
		CurrentAction = PlannedActions.Pop();
		check(CurrentAction != nullptr);
		RegisterNewAction(CurrentAction);
		CurrentAction->StartAction(AIOwner, CurrentState, AIOwner->GetPawn(), AIOwner->GetWorld());
        UE_VLOG(GetOwner(), LogRPAI, Log, TEXT("Action Started %s"), *CurrentAction->GetActionName());
	}
	else
	{
		StartLogic();
	}
}

void URPAIComposerBrainComponent::OnActionCompleted(UReasonablePlanningActionBase* CompletedAction, AAIController* ActionInstigator, UReasonablePlanningState* CompletedOnState)
{
	if (CompletedOnState != CurrentState)
	{
		//log
		return;
	}

	if (CompletedAction != CurrentAction)
	{
		//log
		return;
	}

	if (ActionInstigator != AIOwner)
	{
		//log
		return;
	}

    UE_VLOG(GetOwner(), LogRPAI, Log, TEXT("Action Completed %s"), *CompletedAction->GetActionName());
	PopNextAction();
}

void URPAIComposerBrainComponent::OnActionCancelled(UReasonablePlanningActionBase* CancelledAction, AAIController* ActionInstigator, UReasonablePlanningState* CompletedOnState)
{
	if (CompletedOnState != CurrentState)
	{
		//log
		return;
	}

	if (CancelledAction != CurrentAction)
	{
		//log
		return;
	}

	if (ActionInstigator != AIOwner)
	{
		//log
		return;
	}

	PopNextAction();
}

void URPAIComposerBrainComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bIsPaused)
	{
		return;
	}

	if (CurrentAction != nullptr)
	{
		CurrentAction->UpdateAction(AIOwner, CurrentState, DeltaTime, AIOwner->GetPawn(), AIOwner->GetWorld());
	}
}

void URPAIComposerBrainComponent::SetReasonablePlanningBehavior(UReasonablePlanningAIBehavior* NewBehavior)
{
	if (NewBehavior != ReasonablePlanningBehavior)
	{
		StopLogic("New Behavior");
		ReasonablePlanningBehavior = NewBehavior;
		StartLogic();
	}
}


void URPAIComposerBrainComponent::StartLogic()
{
	UE_VLOG(GetOwner(), LogRPAI, Log, TEXT("%s"), ANSI_TO_TCHAR(__FUNCTION__));
	if (CurrentAction != nullptr && CurrentGoal != nullptr)
	{
		UE_VLOG(GetOwner(), LogRPAI, Log, TEXT("%s: Skipping, logic already started."), ANSI_TO_TCHAR(__FUNCTION__));
		return;
	}

	check(ReasonablePlanningBehavior != nullptr);

	auto Reasoner = ReasonablePlanningBehavior->GetReasoner();
	auto Planner = ReasonablePlanningBehavior->GetPlanner();
	auto Goals = ReasonablePlanningBehavior->GetGoals();
	auto Actions = ReasonablePlanningBehavior->GetActions();

	check(Reasoner != nullptr);
	check(Planner != nullptr);

	CurrentGoal = Reasoner->ReasonNextGoal(Goals, CurrentState);
	if (CurrentGoal != nullptr && Planner->PlanChosenGoal(CurrentGoal, CurrentState, Actions, PlannedActions))
	{
        UE_VLOG(GetOwner(), LogRPAI, Log, TEXT("Goal Planned %s"), *CurrentGoal->GetGoalName());
		PopNextAction();
	}
}

void URPAIComposerBrainComponent::RestartLogic()
{
	UE_VLOG(GetOwner(), LogRPAI, Log, TEXT("%s"), ANSI_TO_TCHAR(__FUNCTION__));
	StopLogic("Restart Logic");
	StartLogic();
}

void URPAIComposerBrainComponent::StopLogic(const FString& Reason)
{
	UE_VLOG(GetOwner(), LogRPAI, Log, TEXT("Stopping Reasonable Planning, reason: \'%s\'"), *Reason);
	PlannedActions.Empty();
	if (CurrentAction != nullptr)
	{
		CurrentAction->CancelAction(AIOwner, CurrentState, AIOwner->GetPawn(), AIOwner->GetWorld());
	}
	CurrentAction = nullptr;
	CurrentGoal = nullptr;
}

void URPAIComposerBrainComponent::Cleanup()
{
	PlannedActions.Empty();
	CurrentAction = nullptr;
	CurrentGoal = nullptr;
}

void URPAIComposerBrainComponent::PauseLogic(const FString& Reason)
{
	UE_VLOG(GetOwner(), LogRPAI, Log, TEXT("Execution updates: PAUSED (%s)"), *Reason);
	bIsPaused = true;
}

EAILogicResuming::Type URPAIComposerBrainComponent::ResumeLogic(const FString& Reason)
{
	UE_VLOG(GetOwner(), LogRPAI, Log, TEXT("Execution updates: RESUMED (%s)"), *Reason);
	bIsPaused = false;
	if (CurrentAction == nullptr || CurrentGoal == nullptr)
	{
		RestartLogic();
		return EAILogicResuming::RestartedInstead;
	}
	return EAILogicResuming::Continue;
}
