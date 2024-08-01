// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/RpaiPlannerBase.h"
#include "Core/RpaiState.h"
#include "RpaiPlanner_AStar.generated.h"

USTRUCT(BlueprintType)
struct FVisitedState
{
	GENERATED_BODY()

	UPROPERTY()
	float Cost; // calculated effort to perform the action

	UPROPERTY()
	float Remaining; // distance left to accomplish the goal

	UPROPERTY()
	int32 StateIndex; // handle to the node state list

	UPROPERTY()
	int32 ParentIndex; // handle to parent in unordered list

	UPROPERTY()
	int32 SelfIndex; // handle to self in unordered list

	UPROPERTY()
	TObjectPtr<URpaiActionBase> Action; //non-unique action performed to reach the above state
};

inline bool operator<(const FVisitedState& LHS, const FVisitedState& RHS)
{
	return LHS.Cost + LHS.Remaining < RHS.Cost + RHS.Remaining;
}

USTRUCT(BlueprintType)
struct FAStarPlannerMemory
{
	GENERATED_BODY()
	
	UPROPERTY()
	TArray<FVisitedState> OpenActions; // all of the open actions to be explored

	UPROPERTY()
	TArray<FVisitedState> ClosedActions; // all of the closed actions terminating

	UPROPERTY()
	TArray<TObjectPtr<URpaiState>> VisitedStates;

	UPROPERTY()
	TArray<FVisitedState> UnorderedNodes;

	UPROPERTY()
	int32 CurrentIterations; // used the track the number of executions to plan

	UPROPERTY()
	URpaiState* FutureState; // cached state scratch pad for projection
};

/**
 * Uses a simple A-Star algorithm to select from the list of given actions to accomplish
 * the given goal based on the current state.
 */
UCLASS()
class REASONABLEPLANNINGAI_API URpaiPlanner_AStar : public URpaiPlannerBase
{
	GENERATED_BODY()

public:
	URpaiPlanner_AStar();

	virtual FString GetDebugInfoString(FRpaiMemoryStruct PlannerMemory) const override;

protected:
	// Used as a method to control planning so we don't go on an exhaustive journey
	UPROPERTY(EditAnywhere, Category = Rpai)
	int32 MaxIterations;

	// Number of iterations to perform per tick until a plan is solved or MaxIterations is capped
	UPROPERTY(EditAnywhere, Category = Rpai)
	int32 IterationsPerTick;

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

private:
	void CleanupInstanceMemory(FAStarPlannerMemory* Memory) const;

	static int32 FindEqualNodeFromState(const URpaiState* Lookup, const TArray<TObjectPtr<URpaiState>>& States, const TArray<FVisitedState>& Nodes);
};
