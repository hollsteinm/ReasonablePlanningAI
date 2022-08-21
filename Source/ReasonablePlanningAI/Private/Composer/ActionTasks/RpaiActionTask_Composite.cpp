// Troll Purse. All rights reserved.


#include "Composer/ActionTasks/RpaiActionTask_Composite.h"

URpaiActionTask_Composite::URpaiActionTask_Composite()
{
	bCompleteAfterStart = false;
}

void URpaiActionTask_Composite::ReceiveStartActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	//Lazy initialization
	for (auto Action : Actions)
	{
		if (!Action->OnActionTaskComplete().IsBoundToObject(this))
		{
			Action->OnActionTaskComplete().AddUObject(this, &URpaiActionTask_Composite::OnActionTaskCompletedOrCancelled);
		}

		if (!Action->OnActionTaskCancelled().IsBoundToObject(this))
		{
			Action->OnActionTaskCancelled().AddUObject(this, &URpaiActionTask_Composite::OnActionTaskCompletedOrCancelled);
		}
	}

	auto& GivenControllerActions = ActiveActions.FindOrAdd(ActionInstigator);
	auto CurrentCount = GivenControllerActions.Num();
	if (CurrentCount > 0)
	{
		for (auto Idx = 0; Idx < CurrentCount; ++Idx)
		{
			GivenControllerActions[Idx]->CancelActionTask(ActionInstigator, CurrentState, ActionTargetActor, ActionWorld);
		}
	}
	GivenControllerActions.Reset(Actions.Num());
	for (auto Action : Actions)
	{
		GivenControllerActions.Add(Action);
		Action->StartActionTask(ActionInstigator, CurrentState, ActionTargetActor, ActionWorld);
	}
}

void URpaiActionTask_Composite::ReceiveUpdateActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, float DeltaSeconds, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	auto AIActiveActions = ActiveActions.Find(ActionInstigator);
	if (AIActiveActions != nullptr)
	{
		for (auto ActiveAction : (*AIActiveActions))
		{
			ActiveAction->UpdateActionTask(ActionInstigator, CurrentState, DeltaSeconds, ActionTargetActor, ActionWorld);
		}
		FlushForController(ActionInstigator);
		if (AIActiveActions->Num() == 0)
		{
			CompleteActionTask(ActionInstigator, CurrentState, ActionTargetActor, ActionWorld);
		}
	}
}

void URpaiActionTask_Composite::ReceiveCancelActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	auto AIActiveActions = ActiveActions.Find(ActionInstigator);
	if (AIActiveActions != nullptr)
	{
		for (auto ActiveAction : (*AIActiveActions))
		{
			ActiveAction->CancelActionTask(ActionInstigator, CurrentState, ActionTargetActor, ActionWorld);
		}
		FlushForController(ActionInstigator);
		ActiveActions.Remove(ActionInstigator);
	}
}

void URpaiActionTask_Composite::ReceiveCompleteActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	auto AIActiveActions = ActiveActions.Find(ActionInstigator);
	if (AIActiveActions != nullptr)
	{
		for (auto ActiveAction : (*AIActiveActions))
		{
			ActiveAction->CompleteActionTask(ActionInstigator, CurrentState, ActionTargetActor, ActionWorld);
		}
		FlushForController(ActionInstigator);
		ActiveActions.Remove(ActionInstigator);
	}
}

void URpaiActionTask_Composite::OnActionTaskCompletedOrCancelled(URpaiComposerActionTaskBase* ActionTask, AAIController* ActionInstigator, URpaiState* CurrentState)
{
	auto AIActiveActions = ActiveActions.Find(ActionInstigator);
	auto& AIFlush = FlushActionIndices.FindOrAdd(ActionInstigator);
	if(AIActiveActions != nullptr)
	{
		auto Idx = AIActiveActions->Find(ActionTask);
		if (Idx != INDEX_NONE)
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
