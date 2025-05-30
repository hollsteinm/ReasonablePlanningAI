// Radaway Software LLC. 2025. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/RpaiReasonerBase.h"
#include "RpaiReasoner_AbsoluteUtility.generated.h"

/**
 * Uses a simple algorithm of determining goal by selecting that which returns the highest utility. Does not consider 0 weight values or those queried as not applicable to the current state. If bUseWeight is false, it will consider 0 category values.
 */
UCLASS()
class REASONABLEPLANNINGAI_API URpaiReasoner_AbsoluteUtility : public URpaiReasonerBase
{
	GENERATED_BODY()
protected:
    virtual URpaiGoalBase* ReceiveReasonNextGoal_Implementation(const TArray<URpaiGoalBase*>& Goals, const URpaiState* CurrentState) const override;
};
