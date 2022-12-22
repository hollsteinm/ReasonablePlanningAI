// Troll Purse. All rights reserved.


#include "Composer/ActionTasks/RpaiActionTask_Sequence.h"
#include "Algo/AllOf.h"

FActionTaskSequence::FActionTaskSequence()
	: ActiveActionTaskSequenceIndex(INDEX_NONE)
	, ActiveActionTaskMemorySlice()
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
			Action->OnActionTaskCancelled().AddUObject(this, &URpaiActionTask_Sequence::OnActionTaskCompletedOrCancelled, ActionMemory, ActionTargetActor, ActionWorld);
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

void URpaiActionTask_Sequence::ReceiveCancelActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	FActionTaskSequence* Memory = ActionMemory.Get<FActionTaskSequence>();
	int32 CurrentIndex = Memory->ActiveActionTaskSequenceIndex;
	if (Actions.IsValidIndex(CurrentIndex))
	{
		Memory->ActiveActionTaskSequenceIndex = INDEX_NONE;
		Actions[CurrentIndex]->CancelActionTask(ActionInstigator, CurrentState, Memory->ActiveActionTaskMemorySlice, ActionTargetActor, ActionWorld);
	}
}

void URpaiActionTask_Sequence::ReceiveCompleteActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	FActionTaskSequence* Memory = ActionMemory.Get<FActionTaskSequence>();
	int32 CurrentIndex = Memory->ActiveActionTaskSequenceIndex;
	if (Actions.IsValidIndex(CurrentIndex))
	{
		Memory->ActiveActionTaskSequenceIndex = CurrentIndex + 1;
		Actions[CurrentIndex]->CompleteActionTask(ActionInstigator, CurrentState, Memory->ActiveActionTaskMemorySlice, ActionTargetActor, ActionWorld);
	}
}

void URpaiActionTask_Sequence::OnActionTaskCompletedOrCancelled(URpaiComposerActionTaskBase* ActionTask, AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	FActionTaskSequence* Memory = ActionMemory.Get<FActionTaskSequence>();
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
