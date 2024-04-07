// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/RpaiPlannerBase.h"
#include "RpaiPlanner_AStar.generated.h"

USTRUCT()
struct FVisitedState
{
	GENERATED_BODY()

	FGuid Id; // identifies the Action as a unique execution of the action
	FGuid ParentId; // the preceeding action that must occur
	float Cost; // calculated effort to perform the action
	float Remaining; // distance left to accomplish the goal
	URpaiState* State; // the mutation of the state if this action was to be performed (high memory cost)
	URpaiActionBase* Action; //non-unique action performed to reach the above state
};

USTRUCT()
struct FAStarPlannerMemory
{
	GENERATED_BODY()

	TArray<FVisitedState> OpenActions; // all of the open actions to be explored
	TArray<FVisitedState> ClosedActions; // all of the closed actions terminating
	int32 CurrentIterations; // used the track the number of executions to plan
	URpaiState* FutureState; // cached state scratch pad for projection
	UObject* DisposableRoot; // used as the root for new Objects
};

/**
 * Uses a simple A-Star algorithm to select from the list of given actions to accomplish
 * the given goal based on the current state.
 */
UCLASS()
class REASONABLEPLANNINGAI_API URpaiPlanner_AStar : public URpaiPlannerBase
{
	GENERATED_BODY()

	URpaiPlanner_AStar();

protected:
	// Used as a method to control planning so we don't go on an exhaustive journey
	UPROPERTY(EditAnywhere, Category = Rpai)
	int32 MaxIterations;

	// Number of iterations to perform per tick until a plan is solved or MaxIterations is capped
	UPROPERTY(EditAnywhere, Category = Rpai)
	int32 IterationsPerTick;

	// Will always attempt to output a plan, even if not a solution
	UPROPERTY(EditAnywhere, Category = Rpai)
	bool bAlwaysHaveAPlan;

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
