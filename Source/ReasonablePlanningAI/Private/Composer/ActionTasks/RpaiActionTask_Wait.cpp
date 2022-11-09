// Troll Purse. All rights reserved.

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
	float WaitTime = FMath::FRandRange(FMath::Max(0.f, WaitTimeSeconds - RandomDeviation), WaitTimeSeconds + RandomDeviation);
	ActionWorld->GetTimerManager().SetTimer(WaitMemory->ActiveHandle, FTimerDelegate::CreateUObject(this, &URpaiActionTask_Wait::CompleteActionTask, ActionInstigator, CurrentState, ActionMemory, ActionTargetActor, ActionWorld), WaitTime, false);
}

void URpaiActionTask_Wait::ReceiveCancelActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld )
{
	ActionWorld->GetTimerManager().ClearTimer(ActionMemory.Get<FActionTaskWait>()->ActiveHandle);
}

