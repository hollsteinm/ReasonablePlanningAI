// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/ReasonablePlanningReasonerBase.h"
#include "Reasoner_AbsoluteUtility.generated.h"

/**
 * Uses a simple algorithm of determining goal by selecting that which returns the highest utility. Does not consider 0 weight values or those queried as not applicable to the current state. If bUseWeight is false, it will consider 0 category values.
 */
UCLASS()
class REASONABLEPLANNINGAI_API UReasoner_AbsoluteUtility : public UReasonablePlanningReasonerBase
{
	GENERATED_BODY()
    
    UReasoner_AbsoluteUtility();
    
public:
    /** If true, will use the weight calculation to determine utility, if false it will use the category to determine weight (fastest). */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="RPAI")
    bool bUseWeight;
	
protected:
    virtual UReasonablePlanningGoalBase* ReceiveReasonNextGoal_Implementation(const TArray<UReasonablePlanningGoalBase*>& Goals, const UReasonablePlanningState* CurrentState) const override;
};
