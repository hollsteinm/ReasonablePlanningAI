// Radaway Software LLC. 2025. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/Mutators/RpaiStateMutator_SetValue.h"
#include "RpaiStateMutator_SetValueVector.generated.h"

/**
 * 
 */
UCLASS()
class REASONABLEPLANNINGAI_API URpaiStateMutator_SetValueVector : public URpaiStateMutator_SetValue
{
	GENERATED_BODY()
	
protected:
    UPROPERTY(EditAnywhere, Category = "Rpai")
    FVector ValueToSet;

    virtual void ReceiveMutate_Implementation(URpaiState* StateToMutate) const override;
    virtual bool CanApply_Implementation(const URpaiState* StateToMutate) const override;

public:
    FORCEINLINE void SetValueToSet(FVector NewValueToSet) { ValueToSet = NewValueToSet; }
};
