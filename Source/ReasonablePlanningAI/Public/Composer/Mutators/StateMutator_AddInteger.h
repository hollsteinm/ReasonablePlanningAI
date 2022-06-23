// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/Mutators/StateMutator_AddValue.h"
#include "StateMutator_AddInteger.generated.h"

/**
 * Add an integer value to a float, integer, or vector. Will attempt a safe operation against floating point values
 */
UCLASS()
class REASONABLEPLANNINGAI_API UStateMutator_AddInteger : public UStateMutator_AddValue
{
	GENERATED_BODY()
    
protected:
    UPROPERTY(EditAnywhere, Category = "RPAI")
    int32 ValueToAdd;
    
    virtual void ReceiveMutate_Implementation(UReasonablePlanningState* StateToMutate) const override;
    virtual bool CanApply_Implementation(const UReasonablePlanningState* StateToMutate) const override;
    
public:
    FORCEINLINE void SetIntegerValueToAdd(int32 NewIntegerValueToAdd) { ValueToAdd = NewIntegerValueToAdd; }
	
};
