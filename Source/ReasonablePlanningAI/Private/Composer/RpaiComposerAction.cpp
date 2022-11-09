// Troll Purse. All rights reserved.


#include "Composer/RpaiComposerAction.h"
#include "Composer/RpaiComposerWeightBase.h"
#include "Composer/RpaiComposerActionTaskBase.h"
#include "Composer/RpaiComposerStateMutator.h"
#include "Composer/RpaiComposerStateQuery.h"
#include "Core/RpaiState.h"

FRpaiMemoryStruct URpaiComposerAction::AllocateMemorySlice(FRpaiMemory& FromMemory) const
{
	if (ActionTask != nullptr)
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
		CompleteAction(ActionInstigator, State, ActionMemory, ActionTargetActor, ActionWorld);
	}
}

float URpaiComposerAction::ReceiveExecutionWeight_Implementation(const URpaiState* GivenState) const
{
	check(WeightAlgorithm != nullptr);
	return WeightAlgorithm->ExecutionWeight(GivenState);
}

void URpaiComposerAction::ReceiveStartAction_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld )
{
	check(ActionTask != nullptr);
	if (LockResourceOnStart.IsNone())
	{
		ActionTask->OnActionTaskComplete().AddUObject(this, &URpaiComposerAction::HandleActionTaskCompleted, ActionMemory, ActionTargetActor, ActionWorld);
		ActionTask->StartActionTask(ActionInstigator, CurrentState, ActionMemory, ActionTargetActor, ActionWorld);
	}
	else
	{
		if (CurrentState->LockResource(LockResourceOnStart, this))
		{
			ActionTask->OnActionTaskComplete().AddUObject(this, &URpaiComposerAction::HandleActionTaskCompleted, ActionMemory, ActionTargetActor, ActionWorld);
			ActionTask->StartActionTask(ActionInstigator, CurrentState, ActionMemory, ActionTargetActor, ActionWorld);
		}
		else
		{
			CancelAction(ActionInstigator, CurrentState, ActionMemory, ActionTargetActor, ActionWorld);
		}
	}
}

void URpaiComposerAction::ReceiveUpdateAction_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, float DeltaSeconds, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	check(ActionTask != nullptr);
	ActionTask->UpdateActionTask(ActionInstigator, CurrentState, DeltaSeconds, ActionMemory, ActionTargetActor, ActionWorld);
}

void URpaiComposerAction::ReceiveCancelAction_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld )
{
	check(ActionTask != nullptr);
	ActionTask->CancelActionTask(ActionInstigator, CurrentState, ActionMemory, ActionTargetActor, ActionWorld);
	if (!LockResourceOnStart.IsNone())
	{
		CurrentState->UnlockResource(LockResourceOnStart, this);
	}
}

void URpaiComposerAction::ReceiveCompleteAction_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	check(ActionTask != nullptr);
	ActionTask->CompleteActionTask(ActionInstigator, CurrentState, ActionMemory, ActionTargetActor, ActionWorld);
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
