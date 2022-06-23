// Troll Purse. All rights reserved.

#include "Composer/ActionTasks/ActionTask_Wait.h"
#include "Core/ReasonablePlanningState.h"
#include "Core/ReasonablePlanningTypes.h"
#include "AIController.h"

UActionTask_Wait::UActionTask_Wait()
	: WaitTimeSeconds(30.f)
	, RandomDeviation(5.f)
{

}

void UActionTask_Wait::ReceiveStartActionTask_Implementation(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	float WaitTime = FMath::FRandRange(FMath::Max(0.f, WaitTimeSeconds - RandomDeviation), WaitTimeSeconds + RandomDeviation);
	FTimerHandle NewHandle;
	ActionWorld->GetTimerManager().SetTimer(NewHandle, FTimerDelegate::CreateUObject(this, &UActionTask_Wait::CompleteActionTask, ActionInstigator, CurrentState, ActionTargetActor, ActionWorld), WaitTime, false);
	ActiveHandles.Emplace(MoveTemp(CurrentState), MoveTemp(NewHandle));
}

void UActionTask_Wait::ReceiveCancelActionTask_Implementation(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld )
{
	FTimerHandle ClearHandle;
	if (ActiveHandles.RemoveAndCopyValue(CurrentState, ClearHandle))
	{
		ActionWorld->GetTimerManager().ClearTimer(ClearHandle);
	}
}

void UActionTask_Wait::ReceiveCompleteActionTask_Implementation(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	ActiveHandles.Remove(CurrentState);
}
