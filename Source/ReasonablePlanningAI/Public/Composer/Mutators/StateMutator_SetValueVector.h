// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/Mutators/StateMutator_SetValue.h"
#include "StateMutator_SetValueVector.generated.h"

/**
 * 
 */
UCLASS()
class REASONABLEPLANNINGAI_API UStateMutator_SetValueVector : public UStateMutator_SetValue
{
	GENERATED_BODY()
	
protected:
    UPROPERTY(EditAnywhere, Category = "RPAI")
    FVector ValueToSet;

    virtual void ReceiveMutate_Implementation(UReasonablePlanningState* StateToMutate) const override;
    virtual bool CanApply_Implementation(const UReasonablePlanningState* StateToMutate) const override;
};
