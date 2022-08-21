// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/RpaiReasonerBase.h"
#include "RpaiReasoner_DualUtility.generated.h"

/**
 * Uses a Dual Utility algorithm to select an appropriate goal. Uses absolute utility selection on goal category. Then uses
 * weight based random selection on goal weight. Will not consider goals that are not applicable for absolute utility. Will
 * not consider goals that return near 0 weight for weight based random selection.
 */
UCLASS()
class REASONABLEPLANNINGAI_API URpaiReasoner_DualUtility : public URpaiReasonerBase
{
	GENERATED_BODY()

protected:
	virtual URpaiGoalBase* ReceiveReasonNextGoal_Implementation(const TArray<URpaiGoalBase*>& Goals, const URpaiState* CurrentState) const override;
	
};
