// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.

#pragma once

#include "Composer/RpaiComposerActionTaskBase.h"
#include "ComposerActionTaskTests.generated.h"

UCLASS(HideDropdown)
class UTestComposerActionTask : public URpaiComposerActionTaskBase
{
   GENERATED_BODY()

private:
   uint32 StartActionTaskCallsTotal;
   uint32 UpdateActionTaskCallsTotal;
   uint32 CancelActionTaskCallsTotal;
   uint32 CompleteActionTaskCallsTotal;

public:
   UTestComposerActionTask();

   uint32 UpdateCallsUntilCompletion;

   FORCEINLINE bool WasStartActionTaskCalledAtLeastOnce() const { return StartActionTaskCallsTotal > 0; }
   FORCEINLINE bool WasUpdateActionTaskCalledAtLeastOnce() const { return UpdateActionTaskCallsTotal > 0; }
   FORCEINLINE bool WasCancelActionTaskCalledAtLeastOnce() const { return CancelActionTaskCallsTotal > 0; }
   FORCEINLINE bool WasCompleteActionTaskCalledAtLeastOnce() const { return CompleteActionTaskCallsTotal > 0; }

   FORCEINLINE bool WasStartActionTaskCalledNever() const { return StartActionTaskCallsTotal == 0; }
   FORCEINLINE bool WasUpdateActionTaskCalledNever()  const { return UpdateActionTaskCallsTotal == 0; }
   FORCEINLINE bool WasCancelActionTaskCalledNever()  const { return CancelActionTaskCallsTotal == 0; }
   FORCEINLINE bool WasCompleteActionTaskCalledNever()  const { return CompleteActionTaskCallsTotal == 0; }

   FORCEINLINE bool WasStartActionTaskCalledNumTimes(uint32 ExpectedCallCount) const { return StartActionTaskCallsTotal == ExpectedCallCount; }
   FORCEINLINE bool WasUpdateActionTaskCalledNumTimes(uint32 ExpectedCallCount) const { return UpdateActionTaskCallsTotal == ExpectedCallCount; }
   FORCEINLINE bool WasCancelActionTaskCalledNumTimes(uint32 ExpectedCallCount) const { return CancelActionTaskCallsTotal == ExpectedCallCount; }
   FORCEINLINE bool WasCompleteActionTaskCalledNumTimes(uint32 ExpectedCallCount) const { return CompleteActionTaskCallsTotal == ExpectedCallCount; }

protected:
   virtual void ReceiveStartActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr) override;
   virtual void ReceiveUpdateActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, float DeltaSeconds, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr) override;
   virtual void ReceiveCancelActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr, bool bCancelShouldExitPlan = true) override;
   virtual void ReceiveCompleteActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr) override;

};