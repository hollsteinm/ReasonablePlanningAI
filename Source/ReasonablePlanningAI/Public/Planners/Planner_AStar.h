// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/ReasonablePlanningPlannerBase.h"
#include "Planner_AStar.generated.h"

/**
 * Uses a simple A-Star algorithm to select from the list of given actions to accomplish
 * the given goal based on the current state.
 */
UCLASS()
class REASONABLEPLANNINGAI_API UPlanner_AStar : public UReasonablePlanningPlannerBase
{
	GENERATED_BODY()

protected:
	virtual bool ReceivePlanChosenGoal_Implementation(const UReasonablePlanningGoalBase* TargetGoal, const UReasonablePlanningState* CurrentState, const TArray<UReasonablePlanningActionBase*>& GivenActions, TArray<UReasonablePlanningActionBase*>& OutActions) const override;
};
