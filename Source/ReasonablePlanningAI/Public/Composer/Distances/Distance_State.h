// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/ReasonablePlanningDistance.h"
#include "Distance_State.generated.h"

/**
 * Determines the distance between two defined state values of the same type.
 */
UCLASS()
class REASONABLEPLANNINGAI_API UDistance_State : public UReasonablePlanningDistance
{
	GENERATED_BODY()
	
protected:
    virtual float ReceiveCalculateDistance_Implementation(const UReasonablePlanningState* GivenState) const override;
    
    /**
    * Right hand state key reference to use
    */
    UPROPERTY(EditDefaultsOnly, Category = "RPAI", meta = (DisplayName = "Right Hand Side State Reference Key"))
    FStateKeyValueReference RHS;
    
public:
    FORCEINLINE void SetRHS(FName Key, EStatePropertyType ExpectedType) { RHS.StateKeyName = Key; RHS.ExpectedValueType = ExpectedType; }
};
