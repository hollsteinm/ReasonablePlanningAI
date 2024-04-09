// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/RpaiPlannerBase.h"
#include "RpaiPlanner_AStar.h"
#include "RpaiPlanner_HUG.generated.h"

USTRUCT()
struct FHugPlannerMemory
{
	GENERATED_BODY()

	TArray<FVisitedState> OpenActions; // all of the open actions to be explored
	TArray<FVisitedState> ClosedActions; // all of the closed actions terminating
	int32 CurrentIterations; // used the track the number of executions to plan
	URpaiState* FutureState; // cached state scratch pad for projection
	UObject* DisposableRoot; // used as the root for new Objects
	float OriginalWeight; // used to detect divergence.
};

/**
 * Hybrid Utility Goal Oriented Action Plan (HUG) as described in the talk "Hybrid Utility GOAP in Unreal Engine" by Martin Hollstein, M+DEV 2023 Madison, WI.
 */
UCLASS()
class REASONABLEPLANNINGAI_API URpaiPlanner_HUG : public URpaiPlannerBase
{
	GENERATED_BODY()

public:
	URpaiPlanner_HUG();

protected:
	// Used as a method to control planning so we don't go on an exhaustive journey
	UPROPERTY(EditAnywhere, Category = Rpai)
	int32 MaxIterations;

	// Number of iterations to perform per tick until a plan is solved or MaxIterations is capped
	UPROPERTY(EditAnywhere, Category = Rpai)
	int32 IterationsPerTick;

	/**
	* Applied to the absolute value of the difference of the starting Goal utility
	* and the utility of a goal at a given node. Once divergence within an
	* acceptable plan is found, the planner will exit success with a short circuited
	* plan. This enables partial plan execution with re-evaluation. This is an
	* emergent version of plan merging.
	*/
	UPROPERTY(EditAnywhere, Category = Rpai)
	float GoalDivergenceThreshold;

	virtual bool ReceivePlanChosenGoal_Implementation(
		const URpaiGoalBase* TargetGoal,
		const URpaiState* CurrentState,
		const TArray<URpaiActionBase*>& GivenActions,
		TArray<URpaiActionBase*>& OutActions
	) const override;

	virtual ERpaiPlannerResult ReceiveStartGoalPlanning_Implementation(
		const URpaiGoalBase* TargetGoal,
		const URpaiState* CurrentState,
		const TArray<URpaiActionBase*>& GivenActions,
		TArray<URpaiActionBase*>& OutActions,
		FRpaiMemoryStruct PlannerMemory
	) const override;

	virtual ERpaiPlannerResult ReceiveTickGoalPlanning_Implementation(
		const URpaiGoalBase* TargetGoal,
		const URpaiState* CurrentState,
		const TArray<URpaiActionBase*>& GivenActions,
		TArray<URpaiActionBase*>& OutActions,
		FRpaiMemoryStruct PlannerMemory
	) const override;

	virtual ERpaiPlannerResult ReceiveCancelGoalPlanning_Implementation(
		const URpaiGoalBase* TargetGoal,
		const URpaiState* CurrentState,
		const TArray<URpaiActionBase*>& GivenActions,
		TArray<URpaiActionBase*>& OutActions,
		FRpaiMemoryStruct PlannerMemory
	) const override;
};
