// Troll Purse. All rights reserved.

#include "Composer/ActionTasks/RpaiActionTask_Wait.h"
#include "Core/RpaiState.h"
#include "Core/RpaiTypes.h"
#include "AIController.h"

URpaiActionTask_Wait::URpaiActionTask_Wait()
	: WaitTimeSeconds(30.f)
	, RandomDeviation(5.f)
{
	bCompleteAfterStart = false;
}

void URpaiActionTask_Wait::ReceiveStartActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	float WaitTime = FMath::FRandRange(FMath::Max(0.f, WaitTimeSeconds - RandomDeviation), WaitTimeSeconds + RandomDeviation);
	FTimerHandle NewHandle;
	ActionWorld->GetTimerManager().SetTimer(NewHandle, FTimerDelegate::CreateUObject(this, &URpaiActionTask_Wait::CompleteActionTask, ActionInstigator, CurrentState, ActionTargetActor, ActionWorld), WaitTime, false);
	ActiveHandles.Emplace(MoveTemp(CurrentState), MoveTemp(NewHandle));
}

void URpaiActionTask_Wait::ReceiveCancelActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld )
{
	FTimerHandle ClearHandle;
	if (ActiveHandles.RemoveAndCopyValue(CurrentState, ClearHandle))
	{
		ActionWorld->GetTimerManager().ClearTimer(ClearHandle);
	}
}

void URpaiActionTask_Wait::ReceiveCompleteActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	ActiveHandles.Remove(CurrentState);
}
