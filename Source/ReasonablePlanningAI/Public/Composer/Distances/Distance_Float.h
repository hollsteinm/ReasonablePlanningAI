// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/ReasonablePlanningDistance.h"
#include "Distance_Float.generated.h"

/**
 * Returns the difference of the LHS side float value to the RHS float value provided.
 */
UCLASS()
class REASONABLEPLANNINGAI_API UDistance_Float : public UReasonablePlanningDistance
{
	GENERATED_BODY()

protected:
    virtual float ReceiveCalculateDistance_Implementation(const UReasonablePlanningState* GivenState) const override;
    
    /**
    * Right hand value to use for comparison to the state value (LHS)
    */
    UPROPERTY(EditDefaultsOnly, Category = "RPAI")
    float RHS;
    
public:
    FORCEINLINE void SetRHS(float NewRHS) { RHS = NewRHS; }
};
