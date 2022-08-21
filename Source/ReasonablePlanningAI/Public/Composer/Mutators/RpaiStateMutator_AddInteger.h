// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/Mutators/RpaiStateMutator_AddValue.h"
#include "RpaiStateMutator_AddInteger.generated.h"

/**
 * Add an integer value to a float, integer, or vector. Will attempt a safe operation against floating point values
 */
UCLASS()
class REASONABLEPLANNINGAI_API URpaiStateMutator_AddInteger : public URpaiStateMutator_AddValue
{
	GENERATED_BODY()
    
protected:
    UPROPERTY(EditAnywhere, Category = "Rpai")
    int32 ValueToAdd;
    
    virtual void ReceiveMutate_Implementation(URpaiState* StateToMutate) const override;
    virtual bool CanApply_Implementation(const URpaiState* StateToMutate) const override;
    
public:
    FORCEINLINE void SetIntegerValueToAdd(int32 NewIntegerValueToAdd) { ValueToAdd = NewIntegerValueToAdd; }
};
