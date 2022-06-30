// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/ReasonablePlanningDistance.h"
#include "Distance_Rotator.generated.h"

/**
 * Determines the distance between to rotators, with the state value being the left hand side value and the property RHS being the right hand side. Distance returned is the Manhattan distance.
 */
UCLASS()
class REASONABLEPLANNINGAI_API UDistance_Rotator : public UReasonablePlanningDistance
{
	GENERATED_BODY()
	
protected:
    virtual float ReceiveCalculateDistance_Implementation(const UReasonablePlanningState* GivenState) const override;
    
    /**
    * Right hand value to use for comparison to the state value (LHS)
    */
    UPROPERTY(EditDefaultsOnly, Category = "RPAI")
    FRotator RHS;
    
public:
    FORCEINLINE void SetRHS(FRotator NewRHS) { RHS = NewRHS; }
};
