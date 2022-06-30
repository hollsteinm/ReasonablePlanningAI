// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/ReasonablePlanningDistance.h"
#include "Distance_Bool.generated.h"

/**
 * Compares a state boolean value to the given boolean value, returns 1.0 if they are the same, 0.0 if they are different.
 */
UCLASS()
class REASONABLEPLANNINGAI_API UDistance_Bool : public UReasonablePlanningDistance
{
	GENERATED_BODY()

protected:
    virtual float ReceiveCalculateDistance_Implementation(const UReasonablePlanningState* GivenState) const override;
    
    /**
    * Right hand value to use for comparison to the state value (LHS)
    */
    UPROPERTY(EditDefaultsOnly, Category = "RPAI")
    bool bRHS;
    
public:
    FORCEINLINE void SetRHS(bool bNewRHS) { bRHS = bNewRHS; }
};
