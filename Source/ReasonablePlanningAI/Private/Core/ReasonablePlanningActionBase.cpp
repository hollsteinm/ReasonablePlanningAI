// Troll Purse. All rights reserved.


#include "Core/ReasonablePlanningActionBase.h"
#include "AIController.h"

void UReasonablePlanningActionBase::ApplyToState(UReasonablePlanningState* GivenState) const
{
	return ReceiveApplyToState(GivenState);
}

bool UReasonablePlanningActionBase::IsApplicable(const UReasonablePlanningState* GivenState) const
{
	return ReceiveIsApplicable(GivenState);
}

float UReasonablePlanningActionBase::ExecutionWeight(const UReasonablePlanningState* GivenState) const
{
	return ReceiveExecutionWeight(GivenState);
}

void UReasonablePlanningActionBase::StartAction(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	ReceiveStartAction(ActionInstigator, CurrentState, ActionTargetActor == nullptr ? ActionInstigator->GetPawn() : ActionTargetActor, ActionWorld == nullptr ? ActionInstigator->GetWorld() : ActionWorld);
	if (OnActionStarted.IsBound())
	{
		OnActionStarted.Broadcast(this, ActionInstigator, CurrentState);
	}
}

void UReasonablePlanningActionBase::UpdateAction(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, float DeltaSeconds, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	ReceiveUpdateAction(ActionInstigator, CurrentState, DeltaSeconds, ActionTargetActor == nullptr ? ActionInstigator->GetPawn() : ActionTargetActor, ActionWorld == nullptr ? ActionInstigator->GetWorld() : ActionWorld);
	if (OnActionUpdated.IsBound())
	{
		OnActionUpdated.Broadcast(this, ActionInstigator, CurrentState, DeltaSeconds);
	}
}

void UReasonablePlanningActionBase::CancelAction(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	ReceiveCancelAction(ActionInstigator, CurrentState, ActionTargetActor == nullptr ? ActionInstigator->GetPawn() : ActionTargetActor, ActionWorld == nullptr ? ActionInstigator->GetWorld() : ActionWorld);
	if (OnActionCancelled.IsBound())
	{
		OnActionCancelled.Broadcast(this, ActionInstigator, CurrentState);
	}
}

void UReasonablePlanningActionBase::CompleteAction(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	ReceiveCompleteAction(ActionInstigator, CurrentState, ActionTargetActor == nullptr ? ActionInstigator->GetPawn() : ActionTargetActor, ActionWorld == nullptr ? ActionInstigator->GetWorld() : ActionWorld);
	if (OnActionComplete.IsBound())
	{
		OnActionComplete.Broadcast(this, ActionInstigator, CurrentState);
	}
}

bool UReasonablePlanningActionBase::IsActionComplete(const AAIController* ActionInstigator, const UReasonablePlanningState* CurrentState, const AActor* ActionTargetActor, const UWorld* ActionWorld) const
{
	return ReceiveIsActionComplete(ActionInstigator, CurrentState, ActionTargetActor == nullptr ? ActionInstigator->GetPawn() : ActionTargetActor, ActionWorld == nullptr ? ActionInstigator->GetWorld() : ActionWorld);
}

void UReasonablePlanningActionBase::ReceiveApplyToState_Implementation(UReasonablePlanningState* GivenState) const
{
	//NOOP
}

bool UReasonablePlanningActionBase::ReceiveIsApplicable_Implementation(const UReasonablePlanningState* GivenState) const
{
	return false; //Never applicable unless overriden
}

float UReasonablePlanningActionBase::ReceiveExecutionWeight_Implementation(const UReasonablePlanningState* GivenState) const
{
	return TNumericLimits<float>::Max();
}

void UReasonablePlanningActionBase::ReceiveStartAction_Implementation(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	//NOOP
}

void UReasonablePlanningActionBase::ReceiveUpdateAction_Implementation(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, float DeltaSeconds, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	//NOOP
}

void UReasonablePlanningActionBase::ReceiveCancelAction_Implementation(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	//NOOP
}

void UReasonablePlanningActionBase::ReceiveCompleteAction_Implementation(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	//NOOP
}

bool UReasonablePlanningActionBase::ReceiveIsActionComplete_Implementation(const AAIController* ActionInstigator, const UReasonablePlanningState* CurrentState, const AActor* ActionTargetActor, const UWorld* ActionWorld) const
{
	return true; //Always auto complete to avoid blocking
}

FString UReasonablePlanningActionBase::GetActionName() const
{
    return ActionName;
}
