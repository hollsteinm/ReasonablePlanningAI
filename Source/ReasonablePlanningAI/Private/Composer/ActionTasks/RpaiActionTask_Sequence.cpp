// Radaway Software LLC. 2025. All rights reserved.


#include "Composer/ActionTasks/RpaiActionTask_Sequence.h"
#include "Algo/AllOf.h"

FActionTaskSequence::FActionTaskSequence()
	: ActiveActionTaskSequenceIndex(INDEX_NONE)
	, ActiveActionTaskMemorySlice()
	, bCancelledFromSequence(false)
{

}

FActionTaskSequence::~FActionTaskSequence()
{
	
}

URpaiActionTask_Sequence::URpaiActionTask_Sequence()
	: SequenceMemoryPool(256)
{
	bCompleteAfterStart = false;
	ActionTaskMemoryStructType = FActionTaskSequence::StaticStruct();
}

void URpaiActionTask_Sequence::ReceiveStartActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	//Lazy initialization
	for (auto& Action : Actions)
	{
		if (!Action->OnActionTaskComplete().IsBoundToObject(this))
		{
			Action->OnActionTaskComplete().AddUObject(this, &URpaiActionTask_Sequence::OnActionTaskCompletedOrCancelled, ActionMemory, ActionTargetActor, ActionWorld);
		}

		if (!Action->OnActionTaskCancelled().IsBoundToObject(this))
		{
			Action->OnActionTaskCancelled().AddUObject(this, &URpaiActionTask_Sequence::OnSequenceActionTaskCancelled, ActionMemory, ActionTargetActor, ActionWorld);
		}
	}

	FActionTaskSequence* Memory = ActionMemory.Get<FActionTaskSequence>();

	if (Actions.Num() > 0)
	{
		Memory->ActiveActionTaskSequenceIndex = 0;
		URpaiComposerActionTaskBase* Next = Actions[Memory->ActiveActionTaskSequenceIndex];
		Memory->ActiveActionTaskMemorySlice = Next->AllocateMemorySlice(SequenceMemoryPool);
		Next->StartActionTask(ActionInstigator, CurrentState, Memory->ActiveActionTaskMemorySlice, ActionTargetActor, ActionWorld);
	}
	else
	{
		CompleteActionTask(ActionInstigator, CurrentState, ActionMemory, ActionTargetActor, ActionWorld);
	}
}

void URpaiActionTask_Sequence::ReceiveUpdateActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, float DeltaSeconds, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	FActionTaskSequence* Memory = ActionMemory.Get<FActionTaskSequence>();
	int32 CurrentIndex = Memory->ActiveActionTaskSequenceIndex;
	if (Actions.IsValidIndex(CurrentIndex))
	{
		URpaiComposerActionTaskBase* Current = Actions[CurrentIndex];
		Actions[CurrentIndex]->UpdateActionTask(ActionInstigator, CurrentState, DeltaSeconds, Memory->ActiveActionTaskMemorySlice, ActionTargetActor, ActionWorld);
	}
}

void URpaiActionTask_Sequence::ReceiveCancelActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld, bool bCancelShouldExitPlan)
{
	FActionTaskSequence* Memory = ActionMemory.Get<FActionTaskSequence>();
	int32 CurrentIndex = Memory->ActiveActionTaskSequenceIndex;
	if (Actions.IsValidIndex(CurrentIndex))
	{
		Memory->ActiveActionTaskSequenceIndex = INDEX_NONE;
		Memory->bCancelledFromSequence = true;
		Actions[CurrentIndex]->CancelActionTask(ActionInstigator, CurrentState, Memory->ActiveActionTaskMemorySlice, ActionTargetActor, ActionWorld);
	}
}

void URpaiActionTask_Sequence::OnSequenceActionTaskCancelled(URpaiComposerActionTaskBase* ActionTask, AAIController* ActionInstigator, URpaiState* CurrentState, bool bCancelShouldExitPlan, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	FActionTaskSequence* Memory = ActionMemory.Get<FActionTaskSequence>();
	if (bCancelShouldExitPlan && !Memory->bCancelledFromSequence)
	{
		CancelActionTask(ActionInstigator, CurrentState, ActionMemory, ActionTargetActor, ActionWorld, bCancelShouldExitPlan);
	}
	else
	{
		OnActionTaskCompletedOrCancelled(ActionTask, ActionInstigator, CurrentState, ActionMemory, ActionTargetActor, ActionWorld);
	}
}

void URpaiActionTask_Sequence::OnActionTaskCompletedOrCancelled(URpaiComposerActionTaskBase* ActionTask, AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	FActionTaskSequence* Memory = ActionMemory.Get<FActionTaskSequence>();
	Memory->ActiveActionTaskSequenceIndex += 1;
	if (Actions.IsValidIndex(Memory->ActiveActionTaskSequenceIndex))
	{
		Memory->ActiveActionTaskMemorySlice = Actions[Memory->ActiveActionTaskSequenceIndex]->AllocateMemorySlice(SequenceMemoryPool);
		Memory->ActiveActionTaskSequenceIndex = Memory->ActiveActionTaskSequenceIndex;
		Actions[Memory->ActiveActionTaskSequenceIndex]->StartActionTask(ActionInstigator, CurrentState, Memory->ActiveActionTaskMemorySlice, ActionTargetActor, ActionWorld);
	}
	else
	{
		CompleteActionTask(ActionInstigator, CurrentState, ActionMemory, ActionTargetActor, ActionWorld);
	}
}

void URpaiActionTask_Sequence::AddActionTaskToSequence(URpaiComposerActionTaskBase* NewAction)
{
	Actions.Add(NewAction);
}

void URpaiActionTask_Sequence::RemoveActionTaskFromSequence(const URpaiComposerActionTaskBase* ActionToRemove)
{
	Actions.RemoveAll([ActionToRemove](const URpaiComposerActionTaskBase* Action) -> bool { return ActionToRemove == Action; });
}

const URpaiComposerActionTaskBase* URpaiActionTask_Sequence::ViewTaskFromSequence(int32 Index) const
{
	return Actions.IsValidIndex(Index) ? Actions[Index] : nullptr;
}