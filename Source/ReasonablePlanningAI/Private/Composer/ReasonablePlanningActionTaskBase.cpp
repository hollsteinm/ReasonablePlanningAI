// Troll Purse. All rights reserved.


#include "Composer/ReasonablePlanningActionTaskBase.h"
#include "AIController.h"

UReasonablePlanningActionTaskBase::UReasonablePlanningActionTaskBase()
	: bCompleteAfterStart(true)
{

}

void UReasonablePlanningActionTaskBase::StartActionTask(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
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

void UReasonablePlanningActionTaskBase::UpdateActionTask(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, float DeltaSeconds, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	ReceiveUpdateActionTask(ActionInstigator, CurrentState, DeltaSeconds, ActionTargetActor == nullptr ? ActionInstigator->GetPawn() : ActionTargetActor, ActionWorld == nullptr ? ActionInstigator->GetWorld() : ActionWorld);
	if (ActionTaskUpdatedEvent.IsBound())
	{
		ActionTaskUpdatedEvent.Broadcast(this, ActionInstigator, CurrentState, DeltaSeconds);
	}
}

void UReasonablePlanningActionTaskBase::CancelActionTask(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	ReceiveCancelActionTask(ActionInstigator, CurrentState, ActionTargetActor == nullptr ? ActionInstigator->GetPawn() : ActionTargetActor, ActionWorld == nullptr ? ActionInstigator->GetWorld() : ActionWorld);
	if (ActionTaskCancelledEvent.IsBound())
	{
		ActionTaskCancelledEvent.Broadcast(this, ActionInstigator, CurrentState);
	}
}

void UReasonablePlanningActionTaskBase::CompleteActionTask(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	ReceiveCompleteActionTask(ActionInstigator, CurrentState, ActionTargetActor == nullptr ? ActionInstigator->GetPawn() : ActionTargetActor, ActionWorld == nullptr ? ActionInstigator->GetWorld() : ActionWorld);
	if (ActionTaskCompletedEvent.IsBound())
	{
		ActionTaskCompletedEvent.Broadcast(this, ActionInstigator, CurrentState);
	}
}

void UReasonablePlanningActionTaskBase::ReceiveStartActionTask_Implementation(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	//NOOP
}

void UReasonablePlanningActionTaskBase::ReceiveUpdateActionTask_Implementation(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, float DeltaSeconds, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	//NOOP
}

void UReasonablePlanningActionTaskBase::ReceiveCancelActionTask_Implementation(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	//NOOP
}

void UReasonablePlanningActionTaskBase::ReceiveCompleteActionTask_Implementation(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	//NOOP
}
