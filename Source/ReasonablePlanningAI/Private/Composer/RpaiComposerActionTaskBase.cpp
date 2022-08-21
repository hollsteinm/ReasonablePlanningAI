// Troll Purse. All rights reserved.


#include "Composer/RpaiComposerActionTaskBase.h"
#include "AIController.h"

URpaiComposerActionTaskBase::URpaiComposerActionTaskBase()
	: bCompleteAfterStart(true)
{

}

void URpaiComposerActionTaskBase::StartActionTask(AAIController* ActionInstigator, URpaiState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	ReceiveStartActionTask(ActionInstigator, CurrentState, ActionTargetActor == nullptr ? ActionInstigator->GetPawn() : ActionTargetActor, ActionWorld == nullptr ? ActionInstigator->GetWorld() : ActionWorld);
	if (ActionTaskStartedEvent.IsBound())
	{
		ActionTaskStartedEvent.Broadcast(this, ActionInstigator, CurrentState);
	}
	if (bCompleteAfterStart)
	{
		CompleteActionTask(ActionInstigator, CurrentState, ActionTargetActor, ActionWorld);
	}
}

void URpaiComposerActionTaskBase::UpdateActionTask(AAIController* ActionInstigator, URpaiState* CurrentState, float DeltaSeconds, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	ReceiveUpdateActionTask(ActionInstigator, CurrentState, DeltaSeconds, ActionTargetActor == nullptr ? ActionInstigator->GetPawn() : ActionTargetActor, ActionWorld == nullptr ? ActionInstigator->GetWorld() : ActionWorld);
	if (ActionTaskUpdatedEvent.IsBound())
	{
		ActionTaskUpdatedEvent.Broadcast(this, ActionInstigator, CurrentState, DeltaSeconds);
	}
}

void URpaiComposerActionTaskBase::CancelActionTask(AAIController* ActionInstigator, URpaiState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	ReceiveCancelActionTask(ActionInstigator, CurrentState, ActionTargetActor == nullptr ? ActionInstigator->GetPawn() : ActionTargetActor, ActionWorld == nullptr ? ActionInstigator->GetWorld() : ActionWorld);
	if (ActionTaskCancelledEvent.IsBound())
	{
		ActionTaskCancelledEvent.Broadcast(this, ActionInstigator, CurrentState);
	}
}

void URpaiComposerActionTaskBase::CompleteActionTask(AAIController* ActionInstigator, URpaiState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	ReceiveCompleteActionTask(ActionInstigator, CurrentState, ActionTargetActor == nullptr ? ActionInstigator->GetPawn() : ActionTargetActor, ActionWorld == nullptr ? ActionInstigator->GetWorld() : ActionWorld);
	if (ActionTaskCompletedEvent.IsBound())
	{
		ActionTaskCompletedEvent.Broadcast(this, ActionInstigator, CurrentState);
	}
}

void URpaiComposerActionTaskBase::ReceiveStartActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	//NOOP
}

void URpaiComposerActionTaskBase::ReceiveUpdateActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, float DeltaSeconds, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	//NOOP
}

void URpaiComposerActionTaskBase::ReceiveCancelActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	//NOOP
}

void URpaiComposerActionTaskBase::ReceiveCompleteActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	//NOOP
}
