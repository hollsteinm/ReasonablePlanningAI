// Troll Purse. All rights reserved.


#include "Composer/RpaiComposerAction.h"
#include "Composer/RpaiComposerWeightBase.h"
#include "Composer/RpaiComposerActionTaskBase.h"
#include "Composer/RpaiComposerStateMutator.h"
#include "Composer/RpaiComposerStateQuery.h"
#include "Core/RpaiState.h"

void URpaiComposerAction::HandleActionTaskCompleted(URpaiComposerActionTaskBase* CompletedActionTask, AAIController* ActionInstigator, URpaiState* State, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	if (CompletedActionTask == ActionTask)
	{
		ActionTask->OnActionTaskComplete().RemoveAll(this);
		CompleteAction(ActionInstigator, State, ActionTargetActor, ActionWorld);
	}
}

float URpaiComposerAction::ReceiveExecutionWeight_Implementation(const URpaiState* GivenState) const
{
	check(WeightAlgorithm != nullptr);
	return WeightAlgorithm->ExecutionWeight(GivenState);
}

void URpaiComposerAction::ReceiveStartAction_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld )
{
	check(ActionTask != nullptr);
	if (LockResourceOnStart.IsNone())
	{
		ActionTask->OnActionTaskComplete().AddUObject(this, &URpaiComposerAction::HandleActionTaskCompleted, ActionTargetActor, ActionWorld);
		ActionTask->StartActionTask(ActionInstigator, CurrentState, ActionTargetActor, ActionWorld);
	}
	else
	{
		if (CurrentState->LockResource(LockResourceOnStart, this))
		{
			ActionTask->OnActionTaskComplete().AddUObject(this, &URpaiComposerAction::HandleActionTaskCompleted, ActionTargetActor, ActionWorld);
			ActionTask->StartActionTask(ActionInstigator, CurrentState, ActionTargetActor, ActionWorld);
		}
		else
		{
			CancelAction(ActionInstigator, CurrentState, ActionTargetActor, ActionWorld);
		}
	}
}

void URpaiComposerAction::ReceiveUpdateAction_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, float DeltaSeconds, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	check(ActionTask != nullptr);
	ActionTask->UpdateActionTask(ActionInstigator, CurrentState, DeltaSeconds, ActionTargetActor, ActionWorld);
}

void URpaiComposerAction::ReceiveCancelAction_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld )
{
	check(ActionTask != nullptr);
	ActionTask->CancelActionTask(ActionInstigator, CurrentState, ActionTargetActor, ActionWorld);
	if (!LockResourceOnStart.IsNone())
	{
		CurrentState->UnlockResource(LockResourceOnStart, this);
	}
}

void URpaiComposerAction::ReceiveCompleteAction_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	check(ActionTask != nullptr);
	ActionTask->CompleteActionTask(ActionInstigator, CurrentState, ActionTargetActor, ActionWorld);
	if (!LockResourceOnStart.IsNone())
	{
		CurrentState->UnlockResource(LockResourceOnStart, this);
	}
}

void URpaiComposerAction::ReceiveApplyToState_Implementation(URpaiState* GivenState) const
{
    for(const auto Mutator : ApplyToStateMutators)
    {
        Mutator->Mutate(GivenState);
    }
}

bool URpaiComposerAction::ReceiveIsApplicable_Implementation(const URpaiState* GivenState) const
{
	check(GivenState != nullptr);
	check(IsApplicableQuery != nullptr);
	return IsApplicableQuery->Query(GivenState) == EStateQueryResult::Succeeded;
}
