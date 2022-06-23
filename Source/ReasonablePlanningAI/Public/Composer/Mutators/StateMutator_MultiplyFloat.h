// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/Mutators/StateMutator_MultiplyValue.h"
#include "StateMutator_MultiplyFloat.generated.h"

/**
 * Multiples the defined state value by the value defined in the MultiplierValue field.
 */
UCLASS()
class REASONABLEPLANNINGAI_API UStateMutator_MultiplyFloat : public UStateMutator_MultiplyValue
{
	GENERATED_BODY()
    
protected:
    /**
     * The multiplier (second part of a multiplication problem) to apply to the multiplicant (first part of multiplication problem) sourced from the state.
     */
    UPROPERTY(EditAnywhere, Category = "RPAI")
    float MultiplierValue;
    
    virtual void ReceiveMutate_Implementation(UReasonablePlanningState* StateToMutate) const override;
    virtual bool CanApply_Implementation(const UReasonablePlanningState* StateToMutate) const override;
    
public:
    FORCEINLINE void SetFloatMultiplierValue(float NewFloatMultiplierValue) { MultiplierValue = NewFloatMultiplierValue; }
};
