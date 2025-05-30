// Radaway Software LLC. 2025. All rights reserved.


#include "Composer/RpaiComposerAction.h"
#include "Composer/RpaiComposerWeightBase.h"
#include "Composer/RpaiComposerActionTaskBase.h"
#include "Composer/RpaiComposerStateMutator.h"
#include "Composer/RpaiComposerStateQuery.h"
#include "Core/RpaiState.h"

FRpaiMemoryStruct URpaiComposerAction::AllocateMemorySlice(FRpaiMemory& FromMemory) const
{
	if (IsValid(ActionTask))
	{
		return ActionTask->AllocateMemorySlice(FromMemory);
	}
	return FRpaiMemoryStruct();
}

void URpaiComposerAction::HandleActionTaskCompleted(URpaiComposerActionTaskBase* CompletedActionTask, AAIController* ActionInstigator, URpaiState* State, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	if (CompletedActionTask == ActionTask)
	{
		ActionTask->OnActionTaskComplete().RemoveAll(this);
		ActionTask->OnActionTaskCancelled().RemoveAll(this);
		CompleteAction(ActionInstigator, State, ActionMemory, ActionTargetActor, ActionWorld);
	}
}

void URpaiComposerAction::HandleActionTaskCancelled(URpaiComposerActionTaskBase* CompletedActionTask, AAIController* ActionInstigator, URpaiState* State, bool bCancelShouldExitPlan, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	if (CompletedActionTask == ActionTask)
	{
		ActionTask->OnActionTaskComplete().RemoveAll(this);
		ActionTask->OnActionTaskCancelled().RemoveAll(this);
		CancelAction(ActionInstigator, State, ActionMemory, ActionTargetActor, ActionWorld, bCancelShouldExitPlan);
	}
}

float URpaiComposerAction::ReceiveExecutionWeight_Implementation(const URpaiState* GivenState) const
{
	return IsValid(WeightAlgorithm) ? WeightAlgorithm->ExecutionWeight(GivenState) : 0.f;
}

void URpaiComposerAction::ReceiveStartAction_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	if (IsValid(ActionTask))
	{
		if (LockResourceOnStart.IsNone())
		{
			ActionTask->OnActionTaskComplete().AddUObject(this, &URpaiComposerAction::HandleActionTaskCompleted, ActionMemory, ActionTargetActor, ActionWorld);
			ActionTask->OnActionTaskCancelled().AddUObject(this, &URpaiComposerAction::HandleActionTaskCancelled, ActionMemory, ActionTargetActor, ActionWorld);
			ActionTask->StartActionTask(ActionInstigator, CurrentState, ActionMemory, ActionTargetActor, ActionWorld);
		}
		else
		{
			if (CurrentState->LockResource(LockResourceOnStart, this))
			{
				ActionTask->OnActionTaskComplete().AddUObject(this, &URpaiComposerAction::HandleActionTaskCompleted, ActionMemory, ActionTargetActor, ActionWorld);
				ActionTask->OnActionTaskCancelled().AddUObject(this, &URpaiComposerAction::HandleActionTaskCancelled, ActionMemory, ActionTargetActor, ActionWorld);
				ActionTask->StartActionTask(ActionInstigator, CurrentState, ActionMemory, ActionTargetActor, ActionWorld);
			}
			else
			{
				CancelAction(ActionInstigator, CurrentState, ActionMemory, ActionTargetActor, ActionWorld);
			}
		}
	}
}

void URpaiComposerAction::ReceiveUpdateAction_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, float DeltaSeconds, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	if (IsValid(ActionTask))
	{
		ActionTask->UpdateActionTask(ActionInstigator, CurrentState, DeltaSeconds, ActionMemory, ActionTargetActor, ActionWorld);
	}
}

void URpaiComposerAction::ReceiveCancelAction_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld, bool bCancelShouldExitPlan)
{
	if (IsValid(ActionTask))
	{
		ActionTask->CancelActionTask(ActionInstigator, CurrentState, ActionMemory, ActionTargetActor, ActionWorld, bCancelShouldExitPlan);
		if (!LockResourceOnStart.IsNone())
		{
			CurrentState->UnlockResource(LockResourceOnStart, this);
		}
	}
}

void URpaiComposerAction::ReceiveCompleteAction_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	if (IsValid(ActionTask))
	{
		ActionTask->CompleteActionTask(ActionInstigator, CurrentState, ActionMemory, ActionTargetActor, ActionWorld);
		if (!LockResourceOnStart.IsNone())
		{
			CurrentState->UnlockResource(LockResourceOnStart, this);
		}
	}
}

void URpaiComposerAction::ReceiveApplyToState_Implementation(URpaiState* GivenState) const
{
    for(const auto Mutator : ApplyToStateMutators)
    {
		if (IsValid(Mutator))
		{
			Mutator->Mutate(GivenState);
		}
    }
}

bool URpaiComposerAction::ReceiveIsApplicable_Implementation(const URpaiState* GivenState) const
{
	check(GivenState != nullptr);
	return IsValid(IsApplicableQuery) ? IsApplicableQuery->Query(GivenState) == EStateQueryResult::Succeeded : false;
}
