// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/Mutators/RpaiStateMutator_AddValue.h"
#include "RpaiStateMutator_AddFloat.generated.h"

/**
 * Add a float value to another float, integer, or vector value within the given state. Will floor float value when applying to integer.
 */
UCLASS()
class REASONABLEPLANNINGAI_API URpaiStateMutator_AddFloat : public URpaiStateMutator_AddValue
{
   GENERATED_BODY()
    
protected:
    /**
     * The value to add to the state property defined in the state property reference
     */
    UPROPERTY(EditAnywhere, Category = "Rpai")
    float ValueToAdd;
    
    virtual void ReceiveMutate_Implementation(URpaiState* StateToMutate) const override;
    virtual bool CanApply_Implementation(const URpaiState* StateToMutate) const override;
    
public:
    FORCEINLINE void SetFloatValueToAdd(float NewFloatValueToAdd) { ValueToAdd = NewFloatValueToAdd; }
   
};
