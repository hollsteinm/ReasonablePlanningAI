// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RpaiPlannerBase.generated.h"

class URpaiGoalBase;
class URpaiActionBase;
class URpaiState;

/**
 * Class the implements the algorithm for planning a given goal, a current state, and a set of actions.
 */
UCLASS(BlueprintType, Blueprintable, Abstract, EditInlineNew, DefaultToInstanced)
class REASONABLEPLANNINGAI_API URpaiPlannerBase : public UObject
{
	GENERATED_BODY()
	
public:
	/**
	* Given a target goal and current state, produce - if possible - a queue of actions to execute in insertion order. Returns false if no solution is found.
	*/
	UFUNCTION(BlueprintPure, Category = "Rpai")
	bool PlanChosenGoal(const URpaiGoalBase* TargetGoal, const URpaiState* CurrentState, const TArray<URpaiActionBase*>& GivenActions, TArray<URpaiActionBase*>& OutActions) const;

protected:
	/**
	* Override this to implement PlanChosenGoal
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Rpai")
	bool ReceivePlanChosenGoal(const URpaiGoalBase* TargetGoal, const URpaiState* CurrentState, const TArray<URpaiActionBase*>& GivenActions, TArray<URpaiActionBase*>& OutActions) const;
	virtual bool ReceivePlanChosenGoal_Implementation(const URpaiGoalBase* TargetGoal, const URpaiState* CurrentState, const TArray<URpaiActionBase*>& GivenActions, TArray<URpaiActionBase*>& OutActions) const;
};
