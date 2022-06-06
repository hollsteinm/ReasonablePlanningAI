// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ReasonablePlanningReasonerBase.generated.h"

class UReasonablePlanningGoalBase;
class UReasonablePlanningState;

/**
 * Class that implements the algorithms for reasoning the next best goal to pursue.
 */
UCLASS(BlueprintType, Blueprintable, Abstract, EditInlineNew, DefaultToInstanced)
class REASONABLEPLANNINGAI_API UReasonablePlanningReasonerBase : public UObject
{
	GENERATED_BODY()
	
public:

	/**
	* Accepts a list of goals and will return the goal with the most dual utility.
	*/
	UFUNCTION(BlueprintPure, Category = "ReasonablePlanning")
	UReasonablePlanningGoalBase* ReasonNextGoal(const TArray<UReasonablePlanningGoalBase*>& Goals, const UReasonablePlanningState* CurrentState) const;

protected:

	/**
	* Override this to implement ReasonNextGoal
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "ReasonablePlanning")
	UReasonablePlanningGoalBase* ReceiveReasonNextGoal(const TArray<UReasonablePlanningGoalBase*>& Goals, const UReasonablePlanningState* CurrentState) const;
	virtual UReasonablePlanningGoalBase* ReceiveReasonNextGoal_Implementation(const TArray<UReasonablePlanningGoalBase*>& Goals, const UReasonablePlanningState* CurrentState) const;
};
