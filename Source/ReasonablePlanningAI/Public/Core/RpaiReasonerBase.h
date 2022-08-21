// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RpaiReasonerBase.generated.h"

class URpaiGoalBase;
class URpaiState;

/**
 * Class that implements the algorithms for reasoning the next best goal to pursue.
 */
UCLASS(BlueprintType, Blueprintable, Abstract, EditInlineNew, DefaultToInstanced)
class REASONABLEPLANNINGAI_API URpaiReasonerBase : public UObject
{
	GENERATED_BODY()
	
public:

	/**
	* Accepts a list of goals and will return the goal with the most dual utility.
	*/
	UFUNCTION(BlueprintPure, Category = "Rpai")
	URpaiGoalBase* ReasonNextGoal(const TArray<URpaiGoalBase*>& Goals, const URpaiState* CurrentState) const;

protected:

	/**
	* Override this to implement ReasonNextGoal
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Rpai")
	URpaiGoalBase* ReceiveReasonNextGoal(const TArray<URpaiGoalBase*>& Goals, const URpaiState* CurrentState) const;
	virtual URpaiGoalBase* ReceiveReasonNextGoal_Implementation(const TArray<URpaiGoalBase*>& Goals, const URpaiState* CurrentState) const;
};
