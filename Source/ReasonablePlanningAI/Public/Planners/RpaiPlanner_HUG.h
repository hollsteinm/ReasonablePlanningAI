// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/RpaiPlannerBase.h"
#include "RpaiPlanner_HUG.generated.h"

/**
 * Hybrid Utility Goal Oriented Action Plan (HUG) as described in the talk "Hybrid Utility GOAP in Unreal Engine" by Martin Hollstein, M+DEV 2023 Madison, WI.
 */
UCLASS()
class REASONABLEPLANNINGAI_API URpaiPlanner_HUG : public URpaiPlannerBase
{
	GENERATED_BODY()

	URpaiPlanner_HUG();

protected:
	/*
	* Upper bound threshold on the number of attempts to plan
	*/
	UPROPERTY(EditAnywhere, Category = Rpai)
	uint32 MaxPlanningAttemptThreshold;

	/**
	* Applied to the absolute value of the difference of the starting Goal utility
	* and the utility of a goal at a given node. Once divergence within an
	* acceptable plan is found, the planner will exit success with a short circuited
	* plan. This enables partial plan execution with re-evaluation. This is an
	* emergent version of plan merging.
	*/
	UPROPERTY(EditAnywhere, Category = Rpai)
	float GoalDivergenceThreshold;

	virtual bool ReceivePlanChosenGoal_Implementation(const URpaiGoalBase* TargetGoal, const URpaiState* CurrentState, const TArray<URpaiActionBase*>& GivenActions, TArray<URpaiActionBase*>& OutActions) const override;
};
