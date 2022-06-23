// Troll Purse. All rights reserved.


#include "Composer/ActionTasks/ActionTask_MakeNoise.h"
#include "AIController.h"

UActionTask_MakeNoise::UActionTask_MakeNoise()
	: Loudness(1.f)
{

}

void UActionTask_MakeNoise::ReceiveStartActionTask_Implementation(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	APawn* AIPawn = ActionInstigator ? ActionInstigator->GetPawn() : nullptr;
	if (AIPawn)
	{
		AIPawn->MakeNoise(Loudness, AIPawn);
	}
}
