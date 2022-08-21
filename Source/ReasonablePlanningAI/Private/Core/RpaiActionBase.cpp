// Troll Purse. All rights reserved.


#include "Core/RpaiActionBase.h"
#include "Core/RpaiTypes.h"
#include "VisualLogger/VisualLoggerTypes.h"
#include "VisualLogger/VisualLogger.h"
#include "AIController.h"

void URpaiActionBase::ApplyToState(URpaiState* GivenState) const
{
	check(GivenState != nullptr);
	return ReceiveApplyToState(GivenState);
}

bool URpaiActionBase::IsApplicable(const URpaiState* GivenState) const
{
	check(GivenState != nullptr);
	return ReceiveIsApplicable(GivenState);
}

float URpaiActionBase::ExecutionWeight(const URpaiState* GivenState) const
{
	check(GivenState != nullptr);
	return ReceiveExecutionWeight(GivenState);
}

void URpaiActionBase::StartAction(AAIController* ActionInstigator, URpaiState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	check(ActionInstigator != nullptr);
	check(CurrentState != nullptr);
	check(ActionTargetActor != nullptr);
	check(ActionWorld != nullptr);

	UE_VLOG(ActionInstigator->GetPawn(), LogRpai, Log, TEXT("Start Action: %s"), *ActionName);
	ReceiveStartAction(ActionInstigator, CurrentState, ActionTargetActor == nullptr ? ActionInstigator->GetPawn() : ActionTargetActor, ActionWorld == nullptr ? ActionInstigator->GetWorld() : ActionWorld);
	if (OnActionStarted.IsBound())
	{
		OnActionStarted.Broadcast(this, ActionInstigator, CurrentState);
	}
}

void URpaiActionBase::UpdateAction(AAIController* ActionInstigator, URpaiState* CurrentState, float DeltaSeconds, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	check(ActionInstigator != nullptr);
	check(CurrentState != nullptr);
	check(ActionTargetActor != nullptr);
	check(ActionWorld != nullptr);

	ReceiveUpdateAction(ActionInstigator, CurrentState, DeltaSeconds, ActionTargetActor == nullptr ? ActionInstigator->GetPawn() : ActionTargetActor, ActionWorld == nullptr ? ActionInstigator->GetWorld() : ActionWorld);
	if (OnActionUpdated.IsBound())
	{
		OnActionUpdated.Broadcast(this, ActionInstigator, CurrentState, DeltaSeconds);
	}
}

void URpaiActionBase::CancelAction(AAIController* ActionInstigator, URpaiState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	check(ActionInstigator != nullptr);
	check(CurrentState != nullptr);
	check(ActionTargetActor != nullptr);
	check(ActionWorld != nullptr);

	UE_VLOG(ActionInstigator->GetPawn(), LogRpai, Log, TEXT("Cancel Action: %s"), *ActionName);
	ReceiveCancelAction(ActionInstigator, CurrentState, ActionTargetActor == nullptr ? ActionInstigator->GetPawn() : ActionTargetActor, ActionWorld == nullptr ? ActionInstigator->GetWorld() : ActionWorld);
	if (OnActionCancelled.IsBound())
	{
		OnActionCancelled.Broadcast(this, ActionInstigator, CurrentState);
	}
}

void URpaiActionBase::CompleteAction(AAIController* ActionInstigator, URpaiState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	check(ActionInstigator != nullptr);
	check(CurrentState != nullptr);
	check(ActionTargetActor != nullptr);
	check(ActionWorld != nullptr);

	UE_VLOG(ActionInstigator->GetPawn(), LogRpai, Log, TEXT("Complete Action: %s"), *ActionName);
	ReceiveCompleteAction(ActionInstigator, CurrentState, ActionTargetActor == nullptr ? ActionInstigator->GetPawn() : ActionTargetActor, ActionWorld == nullptr ? ActionInstigator->GetWorld() : ActionWorld);
	if (OnActionComplete.IsBound())
	{
		OnActionComplete.Broadcast(this, ActionInstigator, CurrentState);
	}
}

bool URpaiActionBase::IsActionComplete(const AAIController* ActionInstigator, const URpaiState* CurrentState, const AActor* ActionTargetActor, const UWorld* ActionWorld) const
{
	check(ActionInstigator != nullptr);
	check(CurrentState != nullptr);
	check(ActionTargetActor != nullptr);
	check(ActionWorld != nullptr);

	return ReceiveIsActionComplete(ActionInstigator, CurrentState, ActionTargetActor == nullptr ? ActionInstigator->GetPawn() : ActionTargetActor, ActionWorld == nullptr ? ActionInstigator->GetWorld() : ActionWorld);
}

void URpaiActionBase::ReceiveApplyToState_Implementation(URpaiState* GivenState) const
{
	//NOOP
}

bool URpaiActionBase::ReceiveIsApplicable_Implementation(const URpaiState* GivenState) const
{
	return false; //Never applicable unless overriden
}

float URpaiActionBase::ReceiveExecutionWeight_Implementation(const URpaiState* GivenState) const
{
	return TNumericLimits<float>::Max();
}

void URpaiActionBase::ReceiveStartAction_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	//NOOP
}

void URpaiActionBase::ReceiveUpdateAction_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, float DeltaSeconds, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	//NOOP
}

void URpaiActionBase::ReceiveCancelAction_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	//NOOP
}

void URpaiActionBase::ReceiveCompleteAction_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	//NOOP
}

bool URpaiActionBase::ReceiveIsActionComplete_Implementation(const AAIController* ActionInstigator, const URpaiState* CurrentState, const AActor* ActionTargetActor, const UWorld* ActionWorld) const
{
	return true; //Always auto complete to avoid blocking
}

FString URpaiActionBase::GetActionName() const
{
	return ActionName.IsEmpty() ? GetNameSafe(this) : ActionName;
}
