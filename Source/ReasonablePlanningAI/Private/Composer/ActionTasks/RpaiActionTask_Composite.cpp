// Troll Purse. All rights reserved.


#include "Composer/ActionTasks/RpaiActionTask_Composite.h"
#include "Algo/AllOf.h"

FActionTaskCompositeMemory::FActionTaskCompositeMemory()
{

}

bool operator==(const FRpaiCompositeActionTaskEntry& LHS, const FRpaiCompositeActionTaskEntry& RHS)
{
	return LHS.Action == RHS.Action;
}

FRpaiCompositeActionTaskEntry::FRpaiCompositeActionTaskEntry()
	: Action(nullptr)
	, bIgnoredForCompositeCompletion(false)
	, bPreferCancelOnCompositeCompletion(false)
{

}

FRpaiCompositeActionTaskEntry::FRpaiCompositeActionTaskEntry(const FRpaiCompositeActionTaskEntry& Other)
	: Action(Other.Action)
	, bIgnoredForCompositeCompletion(Other.bIgnoredForCompositeCompletion)
	, bPreferCancelOnCompositeCompletion(Other.bPreferCancelOnCompositeCompletion)
{

}

URpaiActionTask_Composite::URpaiActionTask_Composite()
	: CompositeMemoryPool(256)
{
	bCompleteAfterStart = false;
	ActionTaskMemoryStructType = FActionTaskCompositeMemory::StaticStruct();
}

void URpaiActionTask_Composite::ReceiveStartActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	//Lazy initialization
	for (auto& Entry : ActionEntries)
	{
		if (!Entry.Action->OnActionTaskComplete().IsBoundToObject(this))
		{
			Entry.Action->OnActionTaskComplete().AddUObject(this, &URpaiActionTask_Composite::OnActionTaskCompletedOrCancelled, ActionMemory);
		}

		if (!Entry.Action->OnActionTaskCancelled().IsBoundToObject(this))
		{
			Entry.Action->OnActionTaskCancelled().AddUObject(this, &URpaiActionTask_Composite::OnActionTaskCompletedOrCancelled, ActionMemory);
		}
	}

	FActionTaskCompositeMemory* Memory = ActionMemory.Get<FActionTaskCompositeMemory>();

	if (!Memory->ActionActionTasks.IsEmpty())
	{
		auto Num = Memory->ActionActionTasks.Num();
		for (auto Idx = 0; Idx < Num; ++Idx)
		{
			Memory->ActionActionTasks[Idx].Action->CancelActionTask(ActionInstigator, CurrentState, Memory->CompositeActionTaskSlices[Idx], ActionTargetActor, ActionWorld);
		}
		
	}

	Memory->ActionActionTasks.Reset(ActionEntries.Num());
	Memory->CompositeActionTaskSlices.Reset(ActionEntries.Num());
	int32 Index = 0;
	for (auto& Entry : ActionEntries)
	{
		Memory->ActionActionTasks.Add(Entry);
		Memory->CompositeActionTaskSlices.Add(Entry.Action->AllocateMemorySlice(CompositeMemoryPool));
		Entry.Action->StartActionTask(ActionInstigator, CurrentState, Memory->CompositeActionTaskSlices[Index], ActionTargetActor, ActionWorld);
		++Index;
	}
}

void URpaiActionTask_Composite::ReceiveUpdateActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, float DeltaSeconds, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	Flush(ActionMemory);
	FActionTaskCompositeMemory* Memory = ActionMemory.Get<FActionTaskCompositeMemory>();
	int32 Index = 0;
	for (auto& Entry : Memory->ActionActionTasks)
	{
		Entry.Action->UpdateActionTask(ActionInstigator, CurrentState, DeltaSeconds, Memory->CompositeActionTaskSlices[Index++], ActionTargetActor, ActionWorld);
	}
	Flush(ActionMemory);
	if (Memory->ActionActionTasks.IsEmpty() || Algo::AllOf(Memory->ActionActionTasks, [](const auto& Entry) { return Entry.bIgnoredForCompositeCompletion; }))
	{
		CompleteActionTask(ActionInstigator, CurrentState, ActionMemory, ActionTargetActor, ActionWorld);
	}
}

void URpaiActionTask_Composite::ReceiveCancelActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	FActionTaskCompositeMemory* Memory = ActionMemory.Get<FActionTaskCompositeMemory>();
	int32 Index = 0;
	for (auto& Entry : Memory->ActionActionTasks)
	{
		Entry.Action->CancelActionTask(ActionInstigator, CurrentState, Memory->CompositeActionTaskSlices[Index++], ActionTargetActor, ActionWorld);
	}
	Flush(ActionMemory);
}

void URpaiActionTask_Composite::ReceiveCompleteActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	FActionTaskCompositeMemory* Memory = ActionMemory.Get<FActionTaskCompositeMemory>();
	int32 Index = 0;
	for (auto Entry : Memory->ActionActionTasks)
	{
		if (Entry.bPreferCancelOnCompositeCompletion)
		{
			Entry.Action->CancelActionTask(ActionInstigator, CurrentState, Memory->CompositeActionTaskSlices[Index++], ActionTargetActor, ActionWorld);
		}
		else
		{
			Entry.Action->CompleteActionTask(ActionInstigator, CurrentState, Memory->CompositeActionTaskSlices[Index++], ActionTargetActor, ActionWorld);
		}
	}
	Flush(ActionMemory);
}

void URpaiActionTask_Composite::OnActionTaskCompletedOrCancelled(URpaiComposerActionTaskBase* ActionTask, AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory)
{
	FActionTaskCompositeMemory* Memory = ActionMemory.Get<FActionTaskCompositeMemory>();
	FRpaiCompositeActionTaskEntry SearchCriteria;
	SearchCriteria.Action = ActionTask;
	int32 Idx = INDEX_NONE;
	if (Memory->ActionActionTasks.Find(SearchCriteria, Idx))
	{
		Memory->FlushActionIndices.Add(Idx);
	}
}

void URpaiActionTask_Composite::Flush(FRpaiMemoryStruct ActionMemory)
{
	static auto const SetSorter = TGreater<int32>();
	FActionTaskCompositeMemory* Memory = ActionMemory.Get<FActionTaskCompositeMemory>();

	if(!Memory->FlushActionIndices.IsEmpty())
	{
		Memory->FlushActionIndices.Sort(SetSorter);
		for (const auto RemoveIdx : Memory->FlushActionIndices)
		{
			Memory->ActionActionTasks.RemoveAt(RemoveIdx, 1, false);
			Memory->CompositeActionTaskSlices.RemoveAt(RemoveIdx, 1, false);
		}
		Memory->FlushActionIndices.Empty();
	}

	Memory->CompositeActionTaskSlices.Shrink();
	Memory->ActionActionTasks.Shrink();
}
