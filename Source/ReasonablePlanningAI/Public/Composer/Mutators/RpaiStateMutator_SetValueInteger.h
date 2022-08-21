// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/Mutators/RpaiStateMutator_SetValue.h"
#include "RpaiStateMutator_SetValueInteger.generated.h"

/**
 * 
 */
UCLASS()
class REASONABLEPLANNINGAI_API URpaiStateMutator_SetValueInteger : public URpaiStateMutator_SetValue
{
	GENERATED_BODY()

protected:
    UPROPERTY(EditAnywhere, Category = "Rpai")
    int32 ValueToSet;

    virtual void ReceiveMutate_Implementation(URpaiState* StateToMutate) const override;
    virtual bool CanApply_Implementation(const URpaiState* StateToMutate) const override;

public:
    FORCEINLINE void SetValueToSet(int32 NewValueToSet) { ValueToSet = NewValueToSet; }
};
