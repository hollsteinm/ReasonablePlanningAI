// Troll Purse. All rights reserved.


#include "Composer/ActionTasks/RpaiActionTask_Sequence.h"
#include "Algo/AllOf.h"

URpaiActionTask_Sequence::URpaiActionTask_Sequence()
{
	bCompleteAfterStart = false;
}

void URpaiActionTask_Sequence::ReceiveStartActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	//Lazy initialization
	for (auto& Action : Actions)
	{
		if (!Action->OnActionTaskComplete().IsBoundToObject(this))
		{
			Action->OnActionTaskComplete().AddUObject(this, &URpaiActionTask_Sequence::OnActionTaskCompletedOrCancelled, ActionTargetActor, ActionWorld);
		}

		if (!Action->OnActionTaskCancelled().IsBoundToObject(this))
		{
			Action->OnActionTaskCancelled().AddUObject(this, &URpaiActionTask_Sequence::OnActionTaskCompletedOrCancelled, ActionTargetActor, ActionWorld);
		}
	}

	auto& CurrentControllerAction = ActiveActionSequenceIndex.FindOrAdd(ActionInstigator, INDEX_NONE);
	if (CurrentControllerAction != INDEX_NONE)
	{
		Actions[CurrentControllerAction]->CancelActionTask(ActionInstigator, CurrentState, ActionTargetActor, ActionWorld);
	}
	if (Actions.Num() > 0)
	{
		CurrentControllerAction = 0;
		Actions[CurrentControllerAction]->StartActionTask(ActionInstigator, CurrentState, ActionTargetActor, ActionWorld);
	}
}

void URpaiActionTask_Sequence::ReceiveUpdateActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, float DeltaSeconds, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	auto CurrentControllerAction = ActiveActionSequenceIndex.Find(ActionInstigator);
	if (CurrentControllerAction != nullptr && Actions.IsValidIndex(*CurrentControllerAction))
	{
		Actions[*CurrentControllerAction]->UpdateActionTask(ActionInstigator, CurrentState, DeltaSeconds, ActionTargetActor, ActionWorld);
	}
}

void URpaiActionTask_Sequence::ReceiveCancelActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	int32 CurrentControllerAction = INDEX_NONE;
	if(ActiveActionSequenceIndex.RemoveAndCopyValue(ActionInstigator, CurrentControllerAction))
	{
		if (CurrentControllerAction < Actions.Num() && CurrentControllerAction != INDEX_NONE)
		{
			Actions[CurrentControllerAction]->CancelActionTask(ActionInstigator, CurrentState, ActionTargetActor, ActionWorld);
		}
	}
}

void URpaiActionTask_Sequence::ReceiveCompleteActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	int32 CurrentControllerAction = INDEX_NONE;
	if (ActiveActionSequenceIndex.RemoveAndCopyValue(ActionInstigator, CurrentControllerAction))
	{
		if (CurrentControllerAction < Actions.Num() && CurrentControllerAction != INDEX_NONE)
		{
			Actions[CurrentControllerAction]->CompleteActionTask(ActionInstigator, CurrentState, ActionTargetActor, ActionWorld);
		}
	}
}

void URpaiActionTask_Sequence::OnActionTaskCompletedOrCancelled(URpaiComposerActionTaskBase* ActionTask, AAIController* ActionInstigator, URpaiState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	auto CurrentControllerAction = ActiveActionSequenceIndex.Find(ActionInstigator);
	if (CurrentControllerAction != nullptr)
	{
		*CurrentControllerAction = *CurrentControllerAction + 1;
		if (!Actions.IsValidIndex(*CurrentControllerAction))
		{
			ActiveActionSequenceIndex.Remove(ActionInstigator);
			CompleteActionTask(ActionInstigator, CurrentState, ActionTargetActor, ActionWorld);
		}
	}
}
