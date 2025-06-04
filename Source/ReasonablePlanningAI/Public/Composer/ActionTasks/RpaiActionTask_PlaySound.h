// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/RpaiComposerActionTaskBase.h"
#include "RpaiActionTask_PlaySound.generated.h"

class USoundCue;

/**
 * Fire and forget a sound, will not wait for the sound to finish playing.
 */
UCLASS()
class REASONABLEPLANNINGAI_API URpaiActionTask_PlaySound : public URpaiComposerActionTaskBase
{
   GENERATED_BODY()

public:
   URpaiActionTask_PlaySound();

protected:
   virtual void ReceiveStartActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr) override;

   UPROPERTY(Category = "Rpai", EditAnywhere)
   USoundCue* SoundToPlay;
   
};
