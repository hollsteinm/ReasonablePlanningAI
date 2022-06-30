// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/ReasonablePlanningDistance.h"
#include "Distance_Vector.generated.h"

/**
 * Determines the distance between the state defined vector value and a defined vector. Will return the distance squared between the two vectors from the state value to the RHS property value.
 */
UCLASS()
class REASONABLEPLANNINGAI_API UDistance_Vector : public UReasonablePlanningDistance
{
	GENERATED_BODY()
	
protected:
    virtual float ReceiveCalculateDistance_Implementation(const UReasonablePlanningState* GivenState) const override;
    
    /**
    * Right hand value to use for comparison to the state value (LHS)
    */
    UPROPERTY(EditDefaultsOnly, Category = "RPAI")
    FVector RHS;
    
public:
    FORCEINLINE void SetRHS(FVector NewRHS) { RHS = NewRHS; }
};
