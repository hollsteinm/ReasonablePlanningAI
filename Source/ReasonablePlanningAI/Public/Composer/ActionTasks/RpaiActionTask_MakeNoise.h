// Radaway Software LLC. 2025. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/RpaiComposerActionTaskBase.h"
#include "RpaiActionTask_MakeNoise.generated.h"

/**
 * Causes an actor controlled by the AI controller to play a noise.
 */
UCLASS()
class REASONABLEPLANNINGAI_API URpaiActionTask_MakeNoise : public URpaiComposerActionTaskBase
{
   GENERATED_BODY()

public:
   URpaiActionTask_MakeNoise();

protected:
   virtual void ReceiveStartActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr) override;

   UPROPERTY(EditAnywhere, meta = (ClampMin = "0", ClampMax = "1", UIMin = "0", UIMax = "1"), Category = "Rpai")
   float Loudness;
};
