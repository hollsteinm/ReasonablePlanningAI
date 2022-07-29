// Troll Purse. All rights reserved.


#include "Composer/ReasonablePlanningAction.h"
#include "Composer/ReasonablePlanningWeightBase.h"
#include "Composer/ReasonablePlanningActionTaskBase.h"
#include "Composer/ReasonablePlanningStateMutator.h"
#include "Composer/ReasonablePlanningStateQuery.h"
#include "Core/ReasonablePlanningState.h"

float UReasonablePlanningAction::ReceiveExecutionWeight_Implementation(const UReasonablePlanningState* GivenState) const
{
	check(WeightAlgorithm != nullptr);
	return WeightAlgorithm->ExecutionWeight(GivenState);
}

void UReasonablePlanningAction::ReceiveStartAction_Implementation(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld )
{
	check(ActionTask != nullptr);
	if (LockResourceOnStart.IsNone())
	{
		ActionTask->StartActionTask(ActionInstigator, CurrentState, ActionTargetActor, ActionWorld);
	}
	else
	{
		if (CurrentState->LockResource(LockResourceOnStart))
		{
			ActionTask->StartActionTask(ActionInstigator, CurrentState, ActionTargetActor, ActionWorld);
		}
		else
		{
			ActionTask->CancelActionTask(ActionInstigator, CurrentState, ActionTargetActor, ActionWorld);
		}
	}
}

void UReasonablePlanningAction::ReceiveUpdateAction_Implementation(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, float DeltaSeconds, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	check(ActionTask != nullptr);
	ActionTask->UpdateActionTask(ActionInstigator, CurrentState, DeltaSeconds, ActionTargetActor, ActionWorld);
}

void UReasonablePlanningAction::ReceiveCancelAction_Implementation(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld )
{
	check(ActionTask != nullptr);
	ActionTask->CancelActionTask(ActionInstigator, CurrentState, ActionTargetActor, ActionWorld);
	if (!LockResourceOnStart.IsNone())
	{
		CurrentState->UnlockResource(LockResourceOnStart);
	}
}

void UReasonablePlanningAction::ReceiveCompleteAction_Implementation(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	check(ActionTask != nullptr);
	ActionTask->CompleteActionTask(ActionInstigator, CurrentState, ActionTargetActor, ActionWorld);
	if (!LockResourceOnStart.IsNone())
	{
		CurrentState->UnlockResource(LockResourceOnStart);
	}
}

void UReasonablePlanningAction::ReceiveApplyToState_Implementation(UReasonablePlanningState* GivenState) const
{
    for(const auto Mutator : ApplyToStateMutators)
    {
        Mutator->Mutate(GivenState);
    }
}

bool UReasonablePlanningAction::ReceiveIsApplicable_Implementation(const UReasonablePlanningState* GivenState) const
{
	check(GivenState != nullptr);
	check(IsApplicableQuery != nullptr);
	return IsApplicableQuery->Query(GivenState) == EStateQueryResult::Succeeded;
}
