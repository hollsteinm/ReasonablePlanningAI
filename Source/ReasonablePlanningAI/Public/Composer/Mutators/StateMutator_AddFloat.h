// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/Mutators/StateMutator_AddValue.h"
#include "StateMutator_AddFloat.generated.h"

/**
 * Add a float value to another float, integer, or vector value within the given state. Will floor float value when applying to integer.
 */
UCLASS()
class REASONABLEPLANNINGAI_API UStateMutator_AddFloat : public UStateMutator_AddValue
{
	GENERATED_BODY()
    
protected:
    /**
     * The value to add to the state property defined in the state property reference
     */
    UPROPERTY(EditAnywhere, Category = "RPAI")
    float ValueToAdd;
    
    virtual void ReceiveMutate_Implementation(UReasonablePlanningState* StateToMutate) const override;
    virtual bool CanApply_Implementation(const UReasonablePlanningState* StateToMutate) const override;
    
public:
    FORCEINLINE void SetFloatValueToAdd(float NewFloatValueToAdd) { ValueToAdd = NewFloatValueToAdd; }
	
};
