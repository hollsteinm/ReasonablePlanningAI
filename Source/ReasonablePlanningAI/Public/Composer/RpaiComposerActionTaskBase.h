// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/RpaiTypes.h"
#include "RpaiComposerActionTaskBase.generated.h"

class URpaiComposerActionTaskBase;
class URpaiState;
class AAIController;
class AActor;
class UWorld;

DECLARE_EVENT_ThreeParams(URpaiState, FActionTaskStarted, URpaiComposerActionTaskBase*, AAIController*, URpaiState*);
DECLARE_EVENT_ThreeParams(URpaiState, FActionTaskCompleted, URpaiComposerActionTaskBase*, AAIController*, URpaiState*);
DECLARE_EVENT_FourParams(URpaiState, FActionTaskCancelled, URpaiComposerActionTaskBase*, AAIController*, URpaiState*, bool);
DECLARE_EVENT_FourParams(URpaiState, FActionTaskUpdated, URpaiComposerActionTaskBase*, AAIController*, URpaiState*, float);

/**
 * A class used as a helper for composition of actions that implements only the
 * action logic of a given action
 */
UCLASS(BlueprintType, Blueprintable, Abstract, EditInlineNew, DefaultToInstanced)
class REASONABLEPLANNINGAI_API URpaiComposerActionTaskBase : public UObject
{
   GENERATED_BODY()

private:
   FActionTaskStarted ActionTaskStartedEvent;
   FActionTaskCompleted ActionTaskCompletedEvent;
   FActionTaskCancelled ActionTaskCancelledEvent;
   FActionTaskUpdated ActionTaskUpdatedEvent;
   
public:
   URpaiComposerActionTaskBase();

   /**
   * The initial logic to run when this action is entered. For actions that do not run over time, this is the appropriate action to call.
   */
   UFUNCTION(BlueprintCallable, Category = "Rpai")
   void StartActionTask(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr);

   /**
   * Subsequent calls to this action state after StartAction has been called and before CancelAction or CompleteAction have been called. Will not execute
   * if IsActionComplete returns true after invoking StartAction.
   */
   UFUNCTION(BlueprintCallable, Category = "Rpai")
   void UpdateActionTask(AAIController* ActionInstigator, URpaiState* CurrentState, float DeltaSeconds, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr);

   /**
   * Immediately terminate the current action and apply any state changes needed to revert this action (if desired).
   */
   UFUNCTION(BlueprintCallable, Category = "Rpai")
   void CancelActionTask(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr, bool bCancelShouldExitPlan = true);

   /**
   * Called immediately after IsActionComplete returns true. Use this to update the state
   */
   UFUNCTION(BlueprintCallable, Category = "Rpai")
   void CompleteActionTask(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr);

   /**
   * Event indicating that the action task has started.
   */
   FActionTaskStarted& OnActionTaskStarted() { return ActionTaskStartedEvent; }

   /**
   * Event indicating that the action task has completed.
   */
   FActionTaskCompleted& OnActionTaskComplete() { return ActionTaskCompletedEvent; }

   /**
   * Event indicating that the action task has been cancelled.
   */
   FActionTaskCancelled& OnActionTaskCancelled() { return ActionTaskCancelledEvent; }

   /**
   * Event indicating that the action task has updated.
   */
   FActionTaskUpdated& OnActionTaskUpdated() { return ActionTaskUpdatedEvent; }

   FRpaiMemoryStruct AllocateMemorySlice(UPARAM(Ref) FRpaiMemory& FromMemory) const;

   uint64 EstimateMemorySize() const;

protected:
   /**
   * Override this to implement StartAction
   */
   UFUNCTION(BlueprintNativeEvent, Category = "ResoanablePlanning")
   void ReceiveStartActionTask(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr);
   virtual void ReceiveStartActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr);

   /**
   * Override this to implement UpdateAction
   */
   UFUNCTION(BlueprintNativeEvent, Category = "Rpai")
   void ReceiveUpdateActionTask(AAIController* ActionInstigator, URpaiState* CurrentState, float DeltaSeconds, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr);
   virtual void ReceiveUpdateActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, float DeltaSeconds, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr);

   /**
   * Override this to implement CancelAction
   */
   UFUNCTION(BlueprintNativeEvent, Category = "Rpai")
   void ReceiveCancelActionTask(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr, bool bCancelShouldExitPlan = true);
   virtual void ReceiveCancelActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr, bool bCancelShouldExitPlan = true);

   /**
   * Override this to implement CompleteAction
   */
   UFUNCTION(BlueprintNativeEvent, Category = "Rpai")
   void ReceiveCompleteActionTask(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr);
   virtual void ReceiveCompleteActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr);

   UPROPERTY(EditAnywhere, Category = "Rpai")
   bool bCompleteAfterStart;

   UPROPERTY(EditDefaultsOnly, Category = "Rpai")
   UScriptStruct* ActionTaskMemoryStructType;
};
