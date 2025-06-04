// Radaway Software LLC. 2025. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/RpaiComposerDistance.h"
#include "RpaiDistance_AddAll.generated.h"

/**
 * All sub distances are returned as a summation. Due to bad design, LHS for this instance is ignored.
 */
UCLASS()
class REASONABLEPLANNINGAI_API URpaiDistance_AddAll : public URpaiComposerDistance
{
   GENERATED_BODY()
   
protected:
   UPROPERTY(EditAnywhere, Category = "Rpai", Instanced)
   TArray<URpaiComposerDistance*> SubDistances;

   virtual float ReceiveCalculateDistance_Implementation(const URpaiState* GivenState) const override;

public:
   FORCEINLINE void SetSubDistances(TArray<URpaiComposerDistance*>&& NewSetSubDistances) { SubDistances = NewSetSubDistances; }
};
