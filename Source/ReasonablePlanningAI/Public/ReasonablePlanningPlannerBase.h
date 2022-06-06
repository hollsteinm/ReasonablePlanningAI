// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ReasonablePlanningPlannerBase.generated.h"

class UReasonablePlanningGoalBase;
class UReasonablePlanningActionBase;
class UReasonablePlanningState;

/**
 * Class the implements the algorithm for planning a given goal, a current state, and a set of actions.
 */
UCLASS(BlueprintType, Blueprintable, Abstract, EditInlineNew, DefaultToInstanced)
class REASONABLEPLANNINGAI_API UReasonablePlanningPlannerBase : public UObject
{
	GENERATED_BODY()
	
public:
	/**
	* Given a target goal and current state, produce - if possible - a queue of actions to execute in insertion order. Returns false if no solution is found.
	*/
	UFUNCTION(BlueprintPure, Category = "ReasonablePlanning")
	bool PlanChosenGoal(const UReasonablePlanningGoalBase* TargetGoal, const UReasonablePlanningState* CurrentState, const TArray<UReasonablePlanningActionBase*>& GivenActions, TArray<UReasonablePlanningActionBase*>& OutActions) const;

protected:
	/**
	* Override this to implement PlanChosenGoal
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "ReasonablePlanning")
	bool ReceivePlanChosenGoal(const UReasonablePlanningGoalBase* TargetGoal, const UReasonablePlanningState* CurrentState, const TArray<UReasonablePlanningActionBase*>& GivenActions, TArray<UReasonablePlanningActionBase*>& OutActions) const;
	virtual bool ReceivePlanChosenGoal_Implementation(const UReasonablePlanningGoalBase* TargetGoal, const UReasonablePlanningState* CurrentState, const TArray<UReasonablePlanningActionBase*>& GivenActions, TArray<UReasonablePlanningActionBase*>& OutActions) const;
};
