// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.


#include "Composer/ActionTasks/RpaiActionTask_MakeNoise.h"
#include "AIController.h"

URpaiActionTask_MakeNoise::URpaiActionTask_MakeNoise()
   : Loudness(1.f)
{

}

void URpaiActionTask_MakeNoise::ReceiveStartActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld)
{
   APawn* AIPawn = ActionInstigator ? ActionInstigator->GetPawn() : nullptr;
   if (AIPawn)
   {
      AIPawn->MakeNoise(Loudness, AIPawn);
   }
}
