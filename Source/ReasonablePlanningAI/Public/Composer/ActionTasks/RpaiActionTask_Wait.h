// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/RpaiComposerActionTaskBase.h"
#include "RpaiActionTask_Wait.generated.h"

USTRUCT(BlueprintType)
struct REASONABLEPLANNINGAI_API FActionTaskWait
{
   GENERATED_BODY()

   FActionTaskWait();

   float RemainingSeconds;
};

/**
 * Wait for the time supplied by the given weight key or defined constant. Supports a random deviation to make things interesting.
 */
UCLASS()
class REASONABLEPLANNINGAI_API URpaiActionTask_Wait : public URpaiComposerActionTaskBase
{
   GENERATED_BODY()
public:
   URpaiActionTask_Wait();

protected:
   virtual void ReceiveStartActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr) override;
   virtual void ReceiveUpdateActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, float DeltaSeconds, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr) override;

   UPROPERTY(Category = "Rpai", EditAnywhere, meta = (ClampMin = "0.0", UIMin = "0.0"))
   float WaitTimeSeconds;

   UPROPERTY(Category = "Rpai", EditAnywhere, meta = (ClampMin = "0.0", UIMin = "0.0"))
   float RandomDeviation;
};
