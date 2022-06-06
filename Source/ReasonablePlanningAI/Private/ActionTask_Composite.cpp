// Troll Purse. All rights reserved.


#include "ActionTask_Composite.h"

UActionTask_Composite::UActionTask_Composite()
{
	bCompleteAfterStart = false;
}

void UActionTask_Composite::ReceiveStartActionTask_Implementation(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	//Lazy initialization
	for (auto Action : Actions)
	{
		if (!Action->OnActionTaskComplete().IsBoundToObject(this))
		{
			Action->OnActionTaskComplete().AddUObject(this, &UActionTask_Composite::OnActionTaskCompletedOrCancelled);
		}

		if (!Action->OnActionTaskCancelled().IsBoundToObject(this))
		{
			Action->OnActionTaskCancelled().AddUObject(this, &UActionTask_Composite::OnActionTaskCompletedOrCancelled);
		}
	}

	auto GivenControllerActions = ActiveActions.Find(ActionInstigator);
	if (GivenControllerActions != nullptr)
	{
		auto CurrentCount = GivenControllerActions->Num();
		if (CurrentCount > 0)
		{
			for (auto Idx = 0; Idx < CurrentCount; ++Idx)
			{
				(*GivenControllerActions)[Idx]->CancelActionTask(ActionInstigator, CurrentState, ActionTargetActor, ActionWorld);
			}
		}
		GivenControllerActions->Reset(Actions.Num());
		for (auto Action : Actions)
		{
			GivenControllerActions->Add(Action);
			Action->StartActionTask(ActionInstigator, CurrentState, ActionTargetActor, ActionWorld);
		}
	}
	else
	{
		for (auto Action : Actions)
		{
			GivenControllerActions->Add(Action);
			Action->StartActionTask(ActionInstigator, CurrentState, ActionTargetActor, ActionWorld);
		}
	}
}

void UActionTask_Composite::ReceiveUpdateActionTask_Implementation(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, float DeltaSeconds, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	auto& AIActiveActions = ActiveActions[ActionInstigator];
	for (auto ActiveAction : AIActiveActions)
	{
		ActiveAction->UpdateActionTask(ActionInstigator, CurrentState, DeltaSeconds, ActionTargetActor, ActionWorld);
	}
	FlushForController(ActionInstigator);
}

void UActionTask_Composite::ReceiveCancelActionTask_Implementation(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	auto& AIActiveActions = ActiveActions[ActionInstigator];
	for (auto ActiveAction : AIActiveActions)
	{
		ActiveAction->CancelActionTask(ActionInstigator, CurrentState, ActionTargetActor, ActionWorld);
	}
	FlushForController(ActionInstigator);
}

void UActionTask_Composite::ReceiveCompleteActionTask_Implementation(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	auto& AIActiveActions = ActiveActions[ActionInstigator];
	for (auto ActiveAction : AIActiveActions)
	{
		ActiveAction->CompleteActionTask(ActionInstigator, CurrentState, ActionTargetActor, ActionWorld);
	}
	FlushForController(ActionInstigator);
}

void UActionTask_Composite::OnActionTaskCompletedOrCancelled(UReasonablePlanningActionTaskBase* ActionTask, AAIController* ActionInstigator, UReasonablePlanningState* CurrentState)
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

void UActionTask_Composite::FlushForController(AAIController* ActionInstigator)
{
	auto AIActiveActions = ActiveActions.Find(ActionInstigator);
	auto AIFlush = FlushActionIndices.Find(ActionInstigator);
	if (AIFlush != nullptr && AIActiveActions != nullptr)
	{
		auto Num = AIFlush->Num();
		for (auto Idx = 0; Idx < Num; ++Idx)
		{
			AIActiveActions->RemoveAt(Idx, 1, false);
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