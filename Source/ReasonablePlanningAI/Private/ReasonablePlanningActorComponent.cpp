// Troll Purse. All rights reserved.


#include "ReasonablePlanningActorComponent.h"
#include "ReasonablePlanningAIBehavior.h"
#include "ReasonablePlanningTypes.h"
#include "ReasonablePlanningActionBase.h"
#include "ReasonablePlanningGoalBase.h"
#include "ReasonablePlanningReasonerBase.h"
#include "ReasonablePlanningPlannerBase.h"
#include "VisualLogger/VisualLoggerTypes.h"
#include "VisualLogger/VisualLogger.h"
#include "AIController.h"

UReasonablePlanningActorComponent::UReasonablePlanningActorComponent()
	: ReasonablePlanningBehavior(nullptr)
	, CurrentAction(nullptr)
	, PlannedActions({})
	, CurrentGoal(nullptr)
	, CurrentState(nullptr)
{
}

void UReasonablePlanningActorComponent::UnregisterOldAction(UReasonablePlanningActionBase* OldAction)
{
	if (OldAction == nullptr)
	{
		return;
	}

	OldAction->OnActionStarted.RemoveAll(this);
	OldAction->OnActionCancelled.RemoveAll(this);
}

void UReasonablePlanningActorComponent::RegisterNewAction(UReasonablePlanningActionBase* NewAction)
{
	if (NewAction == nullptr)
	{
		return;
	}

	NewAction->OnActionComplete.AddUniqueDynamic(this, &UReasonablePlanningActorComponent::OnActionCompleted);
	NewAction->OnActionCancelled.AddUniqueDynamic(this, &UReasonablePlanningActorComponent::OnActionCancelled);
}

void UReasonablePlanningActorComponent::PopNextAction()
{
	if (PlannedActions.Num() > 0)
	{
		UnregisterOldAction(CurrentAction);
		CurrentAction = PlannedActions.Pop();
		check(CurrentAction != nullptr);
		RegisterNewAction(CurrentAction);
		CurrentAction->StartAction(AIOwner, CurrentState, AIOwner->GetPawn(), AIOwner->GetWorld());
	}
	else
	{
		StartLogic();
	}
}

void UReasonablePlanningActorComponent::OnActionCompleted(UReasonablePlanningActionBase* CompletedAction, AAIController* ActionInstigator, UReasonablePlanningState* CompletedOnState)
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

	PopNextAction();
}

void UReasonablePlanningActorComponent::OnActionCancelled(UReasonablePlanningActionBase* CancelledAction, AAIController* ActionInstigator, UReasonablePlanningState* CompletedOnState)
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

void UReasonablePlanningActorComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
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

void UReasonablePlanningActorComponent::SetReasonablePlanningBehavior(UReasonablePlanningAIBehavior* NewBehavior)
{
	if (NewBehavior != ReasonablePlanningBehavior)
	{
		StopLogic("New Behavior");
		ReasonablePlanningBehavior = NewBehavior;
		StartLogic();
	}
}


void UReasonablePlanningActorComponent::StartLogic()
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
		PopNextAction();
	}
}

void UReasonablePlanningActorComponent::RestartLogic()
{
	UE_VLOG(GetOwner(), LogRPAI, Log, TEXT("%s"), ANSI_TO_TCHAR(__FUNCTION__));
	StopLogic("Restart Logic");
	StartLogic();
}

void UReasonablePlanningActorComponent::StopLogic(const FString& Reason)
{
	UE_VLOG(GetOwner(), LogRPAI, Log, TEXT("Stopping BT, reason: \'%s\'"), *Reason);
	PlannedActions.Empty();
	if (CurrentAction != nullptr)
	{
		CurrentAction->CancelAction(AIOwner, CurrentState, AIOwner->GetPawn(), AIOwner->GetWorld());
	}
	CurrentAction = nullptr;
	CurrentGoal = nullptr;
}

void UReasonablePlanningActorComponent::Cleanup()
{
	PlannedActions.Empty();
	CurrentAction = nullptr;
	CurrentGoal = nullptr;
}

void UReasonablePlanningActorComponent::PauseLogic(const FString& Reason)
{
	UE_VLOG(GetOwner(), LogRPAI, Log, TEXT("Execution updates: PAUSED (%s)"), *Reason);
	bIsPaused = true;
}

EAILogicResuming::Type UReasonablePlanningActorComponent::ResumeLogic(const FString& Reason)
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
