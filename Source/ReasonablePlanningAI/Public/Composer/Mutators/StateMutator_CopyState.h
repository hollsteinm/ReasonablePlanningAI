// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/ReasonablePlanningStateMutator.h"
#include "StateMutator_CopyState.generated.h"

/**
 * Copies on state value to another. They must be the same type.
 */
UCLASS()
class REASONABLEPLANNINGAI_API UStateMutator_CopyState : public UReasonablePlanningStateMutator
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "RPAI")
	FStateKeyValueReference StatePropertyToCopy;

	virtual void ReceiveMutate_Implementation(UReasonablePlanningState* StateToChange) const override;
	virtual bool CanApply_Implementation(const UReasonablePlanningState* StateToChange) const override;

public:
	FORCEINLINE void SetCopiedFromStateValue(FName StateKeyName, EStatePropertyType ExpectedValueType) { StatePropertyToCopy.StateKeyName = StateKeyName; StatePropertyToCopy.ExpectedValueType = ExpectedValueType; }
};
