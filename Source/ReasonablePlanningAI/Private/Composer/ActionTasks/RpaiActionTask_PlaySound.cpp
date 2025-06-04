// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.

#include "Composer/ActionTasks/RpaiActionTask_PlaySound.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "AIController.h"
#include "Sound/SoundCue.h"

URpaiActionTask_PlaySound::URpaiActionTask_PlaySound()
   : SoundToPlay(nullptr)
{

}

void URpaiActionTask_PlaySound::ReceiveStartActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld)
{
   if (SoundToPlay != nullptr)
   {
      if (const APawn* AIPawn = ActionInstigator->GetPawn())
      {
         UGameplayStatics::SpawnSoundAttached(SoundToPlay, AIPawn->GetRootComponent());
      }
   }
}
