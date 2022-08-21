// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/Mutators/RpaiStateMutator_MultiplyValue.h"
#include "RpaiStateMutator_MultiplyInteger.generated.h"

/**
 * Multiples the defined state value by the value defined in the MultiplierValue field.
 */
UCLASS()
class REASONABLEPLANNINGAI_API URpaiStateMutator_MultiplyInteger : public URpaiStateMutator_MultiplyValue
{
	GENERATED_BODY()

protected:
    /**
     * The multiplier (second part of a multiplication problem) to apply to the multiplicant (first part of multiplication problem) sourced from the state.
     */
    UPROPERTY(EditAnywhere, Category = "Rpai")
    int32 MultiplierValue;
    
    virtual void ReceiveMutate_Implementation(URpaiState* StateToMutate) const override;
    virtual bool CanApply_Implementation(const URpaiState* StateToMutate) const override;
    
public:
    FORCEINLINE void SetIntegerMultiplierValue(float NewIntegerMultiplierValue) { MultiplierValue = NewIntegerMultiplierValue; }

};
