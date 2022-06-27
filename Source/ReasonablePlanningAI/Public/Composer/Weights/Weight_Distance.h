// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/ReasonablePlanningWeightBase.h"
#include "Weight_Distance.generated.h"

class UReasonablePlanningDistance;

/**
 * A weight adapter that uses a distance algorithm to determine the weight
 */
UCLASS()
class REASONABLEPLANNINGAI_API UWeight_Distance : public UReasonablePlanningWeightBase
{
	GENERATED_BODY()
    
protected:
    UPROPERTY(EditAnywhere, Category="RPAI")
    UReasonablePlanningDistance* Distance;
    
    virtual float ReceiveExecutionWeight_Implementation(const UReasonablePlanningState* GivenState) const override;
    
public:
    FORCEINLINE void SetDistance(UReasonablePlanningDistance* NewDistance) { Distance = NewDistance; }
	
};
