// Radaway Software LLC. 2025. All rights reserved.


#include "Core/RpaiActionBase.h"
#include "Core/RpaiTypes.h"
#include "VisualLogger/VisualLoggerTypes.h"
#include "VisualLogger/VisualLogger.h"
#include "AIController.h"

URpaiActionBase::URpaiActionBase()
   : ActionMemoryStructType(nullptr)
{

}

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

FRpaiMemoryStruct URpaiActionBase::AllocateMemorySlice(FRpaiMemory& FromMemory) const
{
   if (ActionMemoryStructType == nullptr)
   {
      return FRpaiMemoryStruct();
   }
   return FRpaiMemoryStruct(&FromMemory, ActionMemoryStructType);
}

void URpaiActionBase::StartAction(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld)
{
   check(ActionInstigator != nullptr);
   check(CurrentState != nullptr);

   UE_VLOG(ActionInstigator->GetPawn(), LogRpai, Log, TEXT("Start Action: %s"), *ActionName);
   ReceiveStartAction(ActionInstigator, CurrentState, ActionMemory, ActionTargetActor == nullptr ? ActionInstigator->GetPawn() : ActionTargetActor, ActionWorld == nullptr ? ActionInstigator->GetWorld() : ActionWorld);
   if (OnActionStarted.IsBound())
   {
      OnActionStarted.Broadcast(this, ActionInstigator, CurrentState);
   }
}

void URpaiActionBase::UpdateAction(AAIController* ActionInstigator, URpaiState* CurrentState, float DeltaSeconds, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld)
{
   check(ActionInstigator != nullptr);
   check(CurrentState != nullptr);

   ReceiveUpdateAction(ActionInstigator, CurrentState, DeltaSeconds, ActionMemory, ActionTargetActor == nullptr ? ActionInstigator->GetPawn() : ActionTargetActor, ActionWorld == nullptr ? ActionInstigator->GetWorld() : ActionWorld);
   if (OnActionUpdated.IsBound())
   {
      OnActionUpdated.Broadcast(this, ActionInstigator, CurrentState, DeltaSeconds);
   }
}

void URpaiActionBase::CancelAction(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld, bool bCancelShouldExitPlan)
{
   check(ActionInstigator != nullptr);
   check(CurrentState != nullptr);

   UE_VLOG(ActionInstigator->GetPawn(), LogRpai, Log, TEXT("Cancel Action: %s"), *ActionName);
   ReceiveCancelAction(ActionInstigator, CurrentState, ActionMemory, ActionTargetActor == nullptr ? ActionInstigator->GetPawn() : ActionTargetActor, ActionWorld == nullptr ? ActionInstigator->GetWorld() : ActionWorld, bCancelShouldExitPlan);
   if (OnActionCancelled.IsBound())
   {
      OnActionCancelled.Broadcast(this, ActionInstigator, CurrentState, bCancelShouldExitPlan);
   }
}

void URpaiActionBase::CompleteAction(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld)
{
   check(ActionInstigator != nullptr);
   check(CurrentState != nullptr);

   UE_VLOG(ActionInstigator->GetPawn(), LogRpai, Log, TEXT("Complete Action: %s"), *ActionName);
   ReceiveCompleteAction(ActionInstigator, CurrentState, ActionMemory, ActionTargetActor == nullptr ? ActionInstigator->GetPawn() : ActionTargetActor, ActionWorld == nullptr ? ActionInstigator->GetWorld() : ActionWorld);
   if (OnActionComplete.IsBound())
   {
      OnActionComplete.Broadcast(this, ActionInstigator, CurrentState);
   }
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
   return 0.f;
}

void URpaiActionBase::ReceiveStartAction_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld)
{
   //NOOP
}

void URpaiActionBase::ReceiveUpdateAction_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, float DeltaSeconds, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld)
{
   //NOOP
}

void URpaiActionBase::ReceiveCancelAction_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld, bool bCancelShouldExitPlan)
{
   //NOOP
}

void URpaiActionBase::ReceiveCompleteAction_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld)
{
   //NOOP
}

FString URpaiActionBase::GetActionName() const
{
   return ActionName.IsEmpty() ? GetNameSafe(this) : ActionName;
}
