// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/Mutators/RpaiStateMutator_SetValue.h"
#include "RpaiStateMutator_SetValueBool.generated.h"

/**
 * 
 */
UCLASS()
class REASONABLEPLANNINGAI_API URpaiStateMutator_SetValueBool : public URpaiStateMutator_SetValue
{
   GENERATED_BODY()
   
protected:
    UPROPERTY(EditAnywhere, Category = "Rpai")
    bool bValueToSet;

    virtual void ReceiveMutate_Implementation(URpaiState* StateToMutate) const override;
    virtual bool CanApply_Implementation(const URpaiState* StateToMutate) const override;

public:
    FORCEINLINE void SetValueToSet(bool bNewValueToSet) { bValueToSet = bNewValueToSet; }
};
