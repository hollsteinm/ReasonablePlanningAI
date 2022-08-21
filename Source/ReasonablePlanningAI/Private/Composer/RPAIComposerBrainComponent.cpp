// Troll Purse. All rights reserved.


#include "Composer/RpaiComposerBrainComponent.h"
#include "Composer/RpaiComposerBehavior.h"
#include "Core/RpaiTypes.h"
#include "Core/RpaiActionBase.h"
#include "Core/RpaiGoalBase.h"
#include "Core/RpaiReasonerBase.h"
#include "Core/RpaiPlannerBase.h"
#include "Core/RpaiState.h"
#include "VisualLogger/VisualLoggerTypes.h"
#include "VisualLogger/VisualLogger.h"
#include "AIController.h"

URpaiComposerBrainComponent::URpaiComposerBrainComponent()
	: ReasonablePlanningBehavior(nullptr)
	, CurrentAction(nullptr)
	, PlannedActions({})
	, CurrentGoal(nullptr)
	, CachedStateInstance(nullptr)
{
}

void URpaiComposerBrainComponent::UnregisterOldAction(URpaiActionBase* OldAction)
{
	if (OldAction == nullptr)
	{
		return;
	}

	OldAction->OnActionStarted.RemoveAll(this);
	OldAction->OnActionCancelled.RemoveAll(this);
}

void URpaiComposerBrainComponent::RegisterNewAction(URpaiActionBase* NewAction)
{
	if (NewAction == nullptr)
	{
		return;
	}

	NewAction->OnActionComplete.AddUniqueDynamic(this, &URpaiComposerBrainComponent::OnActionCompleted);
	NewAction->OnActionCancelled.AddUniqueDynamic(this, &URpaiComposerBrainComponent::OnActionCancelled);
}

void URpaiComposerBrainComponent::PopNextAction()
{
	if (PlannedActions.Num() > 0)
	{
		UnregisterOldAction(CurrentAction);
		CurrentAction = PlannedActions.Pop();
		check(CurrentAction != nullptr);
		RegisterNewAction(CurrentAction);
		CurrentAction->StartAction(AIOwner, LoadOrCreateStateFromAi(), AIOwner->GetPawn(), AIOwner->GetWorld());
        UE_VLOG(GetOwner(), LogRpai, Log, TEXT("Action Started %s"), *CurrentAction->GetActionName());
	}
	else
	{
		StartLogic();
	}
}

void URpaiComposerBrainComponent::OnActionCompleted(URpaiActionBase* CompletedAction, AAIController* ActionInstigator, URpaiState* CompletedOnState)
{
	if (CompletedOnState != LoadOrCreateStateFromAi())
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

    UE_VLOG(GetOwner(), LogRpai, Log, TEXT("Action Completed %s"), *CompletedAction->GetActionName());
	PopNextAction();
}

void URpaiComposerBrainComponent::OnActionCancelled(URpaiActionBase* CancelledAction, AAIController* ActionInstigator, URpaiState* CompletedOnState)
{
	if (CompletedOnState != LoadOrCreateStateFromAi())
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

void URpaiComposerBrainComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bIsPaused)
	{
		return;
	}

	if (CurrentAction != nullptr)
	{
		CurrentAction->UpdateAction(AIOwner, LoadOrCreateStateFromAi(), DeltaTime, AIOwner->GetPawn(), AIOwner->GetWorld());
	}
	else
	{
		StartLogic();
	}
}

void URpaiComposerBrainComponent::SetReasonablePlanningBehavior(URpaiComposerBehavior* NewBehavior)
{
	if (NewBehavior != ReasonablePlanningBehavior)
	{
		StopLogic("New Behavior");
		ReasonablePlanningBehavior = NewBehavior;
		StartLogic();
	}
}


void URpaiComposerBrainComponent::StartLogic()
{
	UE_VLOG(GetOwner(), LogRpai, Log, TEXT("%s"), ANSI_TO_TCHAR(__FUNCTION__));
	if (CurrentAction != nullptr && CurrentGoal != nullptr)
	{
		UE_VLOG(GetOwner(), LogRpai, Log, TEXT("%s: Skipping, logic already started."), ANSI_TO_TCHAR(__FUNCTION__));
		return;
	}

	check(ReasonablePlanningBehavior != nullptr);

	auto Reasoner = ReasonablePlanningBehavior->GetReasoner();
	auto Planner = ReasonablePlanningBehavior->GetPlanner();
	auto Goals = ReasonablePlanningBehavior->GetGoals();
	auto Actions = ReasonablePlanningBehavior->GetActions();

	check(Reasoner != nullptr);
	check(Planner != nullptr);

	CurrentGoal = Reasoner->ReasonNextGoal(Goals, LoadOrCreateStateFromAi());
	if (CurrentGoal != nullptr && Planner->PlanChosenGoal(CurrentGoal, LoadOrCreateStateFromAi(), Actions, PlannedActions))
	{
        UE_VLOG(GetOwner(), LogRpai, Log, TEXT("Goal Planned %s"), *CurrentGoal->GetGoalName());
		PopNextAction();
	}
	else
	{
		StopLogic("No Plan");
	}
}

void URpaiComposerBrainComponent::RestartLogic()
{
	UE_VLOG(GetOwner(), LogRpai, Log, TEXT("%s"), ANSI_TO_TCHAR(__FUNCTION__));
	StopLogic("Restart Logic");
	StartLogic();
}

void URpaiComposerBrainComponent::StopLogic(const FString& Reason)
{
	UE_VLOG(GetOwner(), LogRpai, Log, TEXT("Stopping Reasonable Planning, reason: \'%s\'"), *Reason);
	PlannedActions.Empty();
	if (CurrentAction != nullptr)
	{
		CurrentAction->CancelAction(AIOwner, LoadOrCreateStateFromAi(), AIOwner->GetPawn(), AIOwner->GetWorld());
	}
	CurrentAction = nullptr;
	CurrentGoal = nullptr;
}

void URpaiComposerBrainComponent::Cleanup()
{
	PlannedActions.Empty();
	CurrentAction = nullptr;
	CurrentGoal = nullptr;
}

void URpaiComposerBrainComponent::PauseLogic(const FString& Reason)
{
	UE_VLOG(GetOwner(), LogRpai, Log, TEXT("Execution updates: PAUSED (%s)"), *Reason);
	bIsPaused = true;
}

EAILogicResuming::Type URpaiComposerBrainComponent::ResumeLogic(const FString& Reason)
{
	UE_VLOG(GetOwner(), LogRpai, Log, TEXT("Execution updates: RESUMED (%s)"), *Reason);
	bIsPaused = false;
	if (CurrentAction == nullptr || CurrentGoal == nullptr)
	{
		RestartLogic();
		return EAILogicResuming::RestartedInstead;
	}
	return EAILogicResuming::Continue;
}

URpaiState* URpaiComposerBrainComponent::LoadOrCreateStateFromAi()
{
	if (CachedStateInstance == nullptr)
	{
		CachedStateInstance = NewObject<URpaiState>(GetAIOwner(), *ReasonablePlanningBehavior->GetConstructedStateType());
	}
	SetStateFromAi(CachedStateInstance);
	return CachedStateInstance;
}

void URpaiComposerBrainComponent::SetStateFromAi_Implementation(URpaiState* StateToModify) const
{

}