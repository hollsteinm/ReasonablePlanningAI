// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/Mutators/StateMutator_SetValue.h"
#include "StateMutator_SetValueString.generated.h"

/**
 * 
 */
UCLASS()
class REASONABLEPLANNINGAI_API UStateMutator_SetValueString : public UStateMutator_SetValue
{
	GENERATED_BODY()
	
protected:
    UPROPERTY(EditAnywhere, Category = "RPAI")
    FString ValueToSet;

    virtual void ReceiveMutate_Implementation(UReasonablePlanningState* StateToMutate) const override;
    virtual bool CanApply_Implementation(const UReasonablePlanningState* StateToMutate) const override;

public:
    FORCEINLINE void SetValueToSet(FString NewValueToSet) { ValueToSet = NewValueToSet; }
};
