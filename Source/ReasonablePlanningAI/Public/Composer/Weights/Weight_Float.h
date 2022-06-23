// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/ReasonablePlanningWeight.h"
#include "Weight_Float.generated.h"

/**
 * Returns the float value from state as the weight for the owning action
 */
UCLASS()
class REASONABLEPLANNINGAI_API UWeight_Float : public UReasonablePlanningWeight
{
	GENERATED_BODY()
	
protected:
	virtual float ReceiveExecutionWeight_Implementation(const UReasonablePlanningState* GivenState) const override;
};
