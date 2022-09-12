// Troll Purse. All rights reserved.


#include "Composer/ActionTasks/RpaiActionTask_Composite.h"
#include "Algo/AllOf.h"

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
{
	bCompleteAfterStart = false;
}

void URpaiActionTask_Composite::ReceiveStartActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	//Lazy initialization
	for (auto& Entry : ActionEntries)
	{
		if (!Entry.Action->OnActionTaskComplete().IsBoundToObject(this))
		{
			Entry.Action->OnActionTaskComplete().AddUObject(this, &URpaiActionTask_Composite::OnActionTaskCompletedOrCancelled);
		}

		if (!Entry.Action->OnActionTaskCancelled().IsBoundToObject(this))
		{
			Entry.Action->OnActionTaskCancelled().AddUObject(this, &URpaiActionTask_Composite::OnActionTaskCompletedOrCancelled);
		}
	}

	auto& GivenControllerActions = ActiveActions.FindOrAdd(ActionInstigator);
	auto CurrentCount = GivenControllerActions.Num();
	if (CurrentCount > 0)
	{
		for (auto Idx = 0; Idx < CurrentCount; ++Idx)
		{
			GivenControllerActions[Idx].Action->CancelActionTask(ActionInstigator, CurrentState, ActionTargetActor, ActionWorld);
		}
	}
	GivenControllerActions.Reset(ActionEntries.Num());
	for (auto& Entry : ActionEntries)
	{
		GivenControllerActions.Add(Entry);
		Entry.Action->StartActionTask(ActionInstigator, CurrentState, ActionTargetActor, ActionWorld);
	}
}

void URpaiActionTask_Composite::ReceiveUpdateActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, float DeltaSeconds, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	auto AIActiveEntries = ActiveActions.Find(ActionInstigator);
	if (AIActiveEntries != nullptr)
	{
		for (auto& Entry : (*AIActiveEntries))
		{
			Entry.Action->UpdateActionTask(ActionInstigator, CurrentState, DeltaSeconds, ActionTargetActor, ActionWorld);
		}
		FlushForController(ActionInstigator);
		if (AIActiveEntries->Num() == 0 || Algo::AllOf(*AIActiveEntries, [](const auto& Entry) { return Entry.bIgnoredForCompositeCompletion; }))
		{
			CompleteActionTask(ActionInstigator, CurrentState, ActionTargetActor, ActionWorld);
		}
	}
}

void URpaiActionTask_Composite::ReceiveCancelActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	auto AIActiveEntries = ActiveActions.Find(ActionInstigator);
	if (AIActiveEntries != nullptr)
	{
		for (auto& Entry : (*AIActiveEntries))
		{
			Entry.Action->CancelActionTask(ActionInstigator, CurrentState, ActionTargetActor, ActionWorld);
		}
		FlushForController(ActionInstigator);
		ActiveActions.Remove(ActionInstigator);
	}
}

void URpaiActionTask_Composite::ReceiveCompleteActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	auto AIActiveEntries = ActiveActions.Find(ActionInstigator);
	if (AIActiveEntries != nullptr)
	{
		for (auto Entry : (*AIActiveEntries))
		{
			if (Entry.bPreferCancelOnCompositeCompletion)
			{
				Entry.Action->CancelActionTask(ActionInstigator, CurrentState, ActionTargetActor, ActionWorld);
			}
			else
			{
				Entry.Action->CompleteActionTask(ActionInstigator, CurrentState, ActionTargetActor, ActionWorld);
			}
		}
		FlushForController(ActionInstigator);
		ActiveActions.Remove(ActionInstigator);
	}
}

void URpaiActionTask_Composite::OnActionTaskCompletedOrCancelled(URpaiComposerActionTaskBase* ActionTask, AAIController* ActionInstigator, URpaiState* CurrentState)
{
	auto AIActiveEntries = ActiveActions.Find(ActionInstigator);
	auto& AIFlush = FlushActionIndices.FindOrAdd(ActionInstigator);
	if(AIActiveEntries != nullptr)
	{
		FRpaiCompositeActionTaskEntry SearchCriteria;
		SearchCriteria.Action = ActionTask;
		int32 Idx = INDEX_NONE;
		if (AIActiveEntries->Find(SearchCriteria, Idx))
		{
			AIFlush.Add(Idx);
		}
	}
}

void URpaiActionTask_Composite::FlushForController(AAIController* ActionInstigator)
{
	static auto const SetSorter = TGreater<int32>();
	auto AIActiveActions = ActiveActions.Find(ActionInstigator);
	auto AIFlush = FlushActionIndices.Find(ActionInstigator);
	if (AIFlush != nullptr && AIActiveActions != nullptr)
	{
		AIFlush->Sort(SetSorter);
		for (const auto RemoveIdx : (*AIFlush))
		{
			AIActiveActions->RemoveAt(RemoveIdx, 1, false);
		}
	}

	if (AIFlush != nullptr)
	{
		AIFlush->Empty();
	}

	if(AIActiveActions != nullptr)
	{
		AIActiveActions->Shrink();
	}
}
