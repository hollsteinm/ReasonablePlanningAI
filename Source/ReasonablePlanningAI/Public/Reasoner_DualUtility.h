// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ReasonablePlanningReasonerBase.h"
#include "Reasoner_DualUtility.generated.h"

/**
 * Uses a Dual Utility algorithm to select an appropriate goal. Uses absolute utility selection on goal category. Then uses
 * weight based random selection on goal weight. Will not consider goals that are not applicable for absolute utility. Will
 * not consider goals that return near 0 weight for weight based random selection.
 */
UCLASS()
class REASONABLEPLANNINGAI_API UReasoner_DualUtility : public UReasonablePlanningReasonerBase
{
	GENERATED_BODY()

protected:
	virtual UReasonablePlanningGoalBase* ReceiveReasonNextGoal_Implementation(const TArray<UReasonablePlanningGoalBase*>& Goals, const UReasonablePlanningState* CurrentState) const override;
	
};
