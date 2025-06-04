// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.


#include "Composer/RpaiComposerActionTaskBase.h"
#include "AIController.h"

URpaiComposerActionTaskBase::URpaiComposerActionTaskBase()
   : bCompleteAfterStart(true)
   , ActionTaskMemoryStructType(nullptr)
{

}

FRpaiMemoryStruct URpaiComposerActionTaskBase::AllocateMemorySlice(FRpaiMemory& FromMemory) const
{
   if (ActionTaskMemoryStructType != nullptr)
   {
      return FRpaiMemoryStruct(&FromMemory, ActionTaskMemoryStructType);
   }
   return FRpaiMemoryStruct();
}

uint64 URpaiComposerActionTaskBase::EstimateMemorySize() const
{
   return ActionTaskMemoryStructType->GetStructureSize(); // Returns aligned size
}

void URpaiComposerActionTaskBase::StartActionTask(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld)
{
   ReceiveStartActionTask(ActionInstigator, CurrentState, ActionMemory, ActionTargetActor == nullptr ? ActionInstigator->GetPawn() : ActionTargetActor, ActionWorld == nullptr ? ActionInstigator->GetWorld() : ActionWorld);
   if (ActionTaskStartedEvent.IsBound())
   {
      ActionTaskStartedEvent.Broadcast(this, ActionInstigator, CurrentState);
   }
   if (bCompleteAfterStart)
   {
      CompleteActionTask(ActionInstigator, CurrentState, ActionMemory, ActionTargetActor, ActionWorld);
   }
}

void URpaiComposerActionTaskBase::UpdateActionTask(AAIController* ActionInstigator, URpaiState* CurrentState, float DeltaSeconds, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld)
{
   ReceiveUpdateActionTask(ActionInstigator, CurrentState, DeltaSeconds, ActionMemory, ActionTargetActor == nullptr ? ActionInstigator->GetPawn() : ActionTargetActor, ActionWorld == nullptr ? ActionInstigator->GetWorld() : ActionWorld);
   if (ActionTaskUpdatedEvent.IsBound())
   {
      ActionTaskUpdatedEvent.Broadcast(this, ActionInstigator, CurrentState, DeltaSeconds);
   }
}

void URpaiComposerActionTaskBase::CancelActionTask(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld, bool bCancelShouldExitPlan)
{
   ReceiveCancelActionTask(ActionInstigator, CurrentState, ActionMemory, ActionTargetActor == nullptr ? ActionInstigator->GetPawn() : ActionTargetActor, ActionWorld == nullptr ? ActionInstigator->GetWorld() : ActionWorld, bCancelShouldExitPlan);
   if (ActionTaskCancelledEvent.IsBound())
   {
      ActionTaskCancelledEvent.Broadcast(this, ActionInstigator, CurrentState, bCancelShouldExitPlan);
   }
}

void URpaiComposerActionTaskBase::CompleteActionTask(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld)
{
   ReceiveCompleteActionTask(ActionInstigator, CurrentState, ActionMemory, ActionTargetActor == nullptr ? ActionInstigator->GetPawn() : ActionTargetActor, ActionWorld == nullptr ? ActionInstigator->GetWorld() : ActionWorld);
   if (ActionTaskCompletedEvent.IsBound())
   {
      ActionTaskCompletedEvent.Broadcast(this, ActionInstigator, CurrentState);
   }
}

void URpaiComposerActionTaskBase::ReceiveStartActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld)
{
   //NOOP
}

void URpaiComposerActionTaskBase::ReceiveUpdateActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, float DeltaSeconds, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld)
{
   //NOOP
}

void URpaiComposerActionTaskBase::ReceiveCancelActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld, bool bCancelShouldExitPlan)
{
   //NOOP
}

void URpaiComposerActionTaskBase::ReceiveCompleteActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld)
{
   //NOOP
}
