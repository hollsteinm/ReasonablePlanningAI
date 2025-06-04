// Radaway Software LLC. 2025. All rights reserved.

#include "Composer/ActionTasks/RpaiActionTask_Wait.h"
#include "Core/RpaiState.h"
#include "Core/RpaiTypes.h"
#include "AIController.h"

FActionTaskWait::FActionTaskWait()
{

}

URpaiActionTask_Wait::URpaiActionTask_Wait()
   : WaitTimeSeconds(30.f)
   , RandomDeviation(5.f)
{
   bCompleteAfterStart = false;
   ActionTaskMemoryStructType = FActionTaskWait::StaticStruct();
}

void URpaiActionTask_Wait::ReceiveStartActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld)
{
   FActionTaskWait* WaitMemory = ActionMemory.Get<FActionTaskWait>();
   WaitMemory->RemainingSeconds = FMath::FRandRange(FMath::Max(0.f, WaitTimeSeconds - RandomDeviation), WaitTimeSeconds + RandomDeviation);
}

void URpaiActionTask_Wait::ReceiveUpdateActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, float DeltaSeconds, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld)
{
   FActionTaskWait* WaitMemory = ActionMemory.Get<FActionTaskWait>();
   WaitMemory->RemainingSeconds -= DeltaSeconds;
   if (WaitMemory->RemainingSeconds <= 0.f)
   {
      CompleteActionTask(ActionInstigator, CurrentState, ActionMemory, ActionTargetActor, ActionWorld);
   }
}

