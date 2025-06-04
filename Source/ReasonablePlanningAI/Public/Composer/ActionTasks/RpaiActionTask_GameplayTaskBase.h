// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTaskOwnerInterface.h"
#include "Composer/RpaiComposerActionTaskBase.h"
#include "RpaiActionTask_GameplayTaskBase.generated.h"

class UAITask;

USTRUCT(BlueprintType)
struct REASONABLEPLANNINGAI_API FActionTaskGameplayTaskBaseMemory
{
   GENERATED_BODY()

   FActionTaskGameplayTaskBaseMemory();

   UAITask* AITask;
   URpaiState* State;
};

/**
 * Action Task that will execute an AITask. Override (Receive)StartActionTask(_Implementation) and use UAITask::NewAITask(...) to create your
 * new task and fill out the parameters. Use StartTask(...) to enqueue the task for execution. Each instance of URpaiActionTask_GameplayTaskBase
 * and all children only support a 1:1 mapping of state to task. Any exsisting tasks will be cancelled if found associated to the given state.
 */
UCLASS(Abstract)
class REASONABLEPLANNINGAI_API URpaiActionTask_GameplayTaskBase : public URpaiComposerActionTaskBase, public IGameplayTaskOwnerInterface
{
   GENERATED_BODY()

public:
   URpaiActionTask_GameplayTaskBase();

   // BEGIN IGameplayTaskOwnerInterface
   virtual UGameplayTasksComponent* GetGameplayTasksComponent(const UGameplayTask& Task) const override;
   virtual AActor* GetGameplayTaskOwner(const UGameplayTask* Task) const override;
   virtual AActor* GetGameplayTaskAvatar(const UGameplayTask* Task) const override;
   virtual uint8 GetGameplayTaskDefaultPriority() const override;
   virtual void OnGameplayTaskInitialized(UGameplayTask& Task) override;
   virtual void OnGameplayTaskDeactivated(UGameplayTask& Task) override;
   // END IGameplayTaskOwnerInterface

protected:
   virtual void ReceiveCancelActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr, bool bCancelShouldExitPlan = true) override;

   UFUNCTION(BlueprintCallable, Category=Rpai)
   void StartTask(URpaiState* CurrentState, UAITask* TaskToStart, FRpaiMemoryStruct ActionMemory);

   // Gets the Rpai Memory associated to a controller. This should be rarely used, but may be required by child classes. Returns true if memory found, false otherwise.
   UFUNCTION(BlueprintCallable, Category = Rpai)
   bool GetMemoryForController(AAIController* ControllerToQuery, FRpaiMemoryStruct& OutMemoryStruct);

private:
   // Find Rpai Memory associated with a given task.
   TMap<AAIController*, FRpaiMemoryStruct> ControllerToMemory;
};
