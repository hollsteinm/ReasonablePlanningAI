// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/RpaiComposerDistance.h"
#include "RpaiDistance_State.generated.h"

/**
 * Determines the distance between two defined state values of the same type.
 */
UCLASS()
class REASONABLEPLANNINGAI_API URpaiDistance_State : public URpaiComposerDistance
{
	GENERATED_BODY()
	
protected:
    virtual float ReceiveCalculateDistance_Implementation(const URpaiState* GivenState) const override;
    
    /**
    * Right hand state key reference to use
    */
    UPROPERTY(EditAnywhere, Category = "Rpai", meta = (DisplayName = "Right Hand Side State Reference Key"))
    FStateKeyValueReference RHS;
    
public:
    FORCEINLINE void SetRHS(FName Key, EStatePropertyType ExpectedType) { RHS.StateKeyName = Key; RHS.ExpectedValueType = ExpectedType; }
};
