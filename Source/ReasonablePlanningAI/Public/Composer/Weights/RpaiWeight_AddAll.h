// Radaway Software LLC. 2025. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/RpaiComposerWeightBase.h"
#include "RpaiWeight_AddAll.generated.h"

/**
 * Returns the summation of all the SubWeights.
 */
UCLASS()
class REASONABLEPLANNINGAI_API URpaiWeight_AddAll : public URpaiComposerWeightBase
{
   GENERATED_BODY()

protected:
   UPROPERTY(EditAnywhere,Category="Rpai",Instanced)
   TArray<URpaiComposerWeightBase*> SubWeights;

   virtual float ReceiveExecutionWeight_Implementation(const URpaiState* GivenState) const override;

public:
   FORCEINLINE void SetSubWeights(TArray<URpaiComposerWeightBase*>&& NewSubWeights) { SubWeights = NewSubWeights; }
};
