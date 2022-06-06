// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ReasonablePlanningGoalBase.generated.h"

class UReasonablePlanningState;

/**
 * A goal is the system that determines what the overall desired state is
 * and calculates how much work it will take to get from a given current state
 * to a given desired state. A goal will also have reasoning components to it.
 * These components are a category weight and a dynamic weight function. A goal
 * determines utility - how useful a given goal is in a given situation.
 */
UCLASS(BlueprintType, Blueprintable, Abstract, EditInlineNew, DefaultToInstanced)
class REASONABLEPLANNINGAI_API UReasonablePlanningGoalBase : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, BlueprintGetter = GetCategory, Category = "ReasonablePlanning")
	int32 Category;

public:
	UReasonablePlanningGoalBase();

	/**
	* Simple binary function to indicate whether a desired state for this goal
	* has been acheived. As it is a pure function, this can be used to determine
	* a real state or a desired state.
	*/
	UFUNCTION(BlueprintPure, Category="ReasonablePlanning")
	bool IsInDesiredState(const UReasonablePlanningState* GivenState) const;

	/**
	* Determines how much effort it will take to get to the given desired state.
	*/
	UFUNCTION(BlueprintPure, Category = "ReasonablePlanning")
	float GetDistanceToDesiredState(const UReasonablePlanningState* GivenState) const;

	/**
	* Determines whether the utility of this goal is even possible or relevant given
	* the current situtation/state.
	*/
	UFUNCTION(BlueprintPure, Category = "ReasonablePlanning")
	bool IsApplicable(const UReasonablePlanningState* GivenState) const;

	/**
	* Returns some arbitrary grouping value to use for reasoning. Should match
	* the value used by other goals to indicate importance, where lower values
	* are preferred goals.
	*/
	UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "ReasonablePlanning")
	FORCEINLINE int32 GetCategory() const { return Category; }

	/**
	* A more dynamic function used for reasoning, this is a weight to indicate
	* the desirablility, or utility, of the goal based on the current state
	* given to the goal.
	*/
	UFUNCTION(BlueprintPure, Category = "ReasonablePlanning")
	float GetWeight(const UReasonablePlanningState* GivenState) const;
	
protected:
	/**
	* Override this to implement IsInDesiredState
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "ReasonablePlanning")
	bool ReceiveIsInDesiredState(const UReasonablePlanningState* CurrentState) const;
	virtual bool ReceiveIsInDesiredState_Implementation(const UReasonablePlanningState* GivenState) const;

	/**
	* Override this to implement GetDistanceToDesiredState. Lower distances indicate less work
	* to proceed to the desired state.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "ReasonablePlanning")
	float ReceiveGetDistanceToDesiredState(const UReasonablePlanningState* CurrentState) const;
	virtual float ReceiveGetDistanceToDesiredState_Implementation(const UReasonablePlanningState* GivenState) const;

	/**
	* Override this to implement IsApplicable
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "ReasonablePlanning")
	bool ReceiveIsApplicable(const UReasonablePlanningState* CurrentState) const;
	virtual bool ReceiveIsApplicable_Implementation(const UReasonablePlanningState* GivenState) const;

	/**
	* Override this to implement GetWeight. Higher weights indicate more utility (more desired to execute).
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "ReasonablePlanning")
	float ReceiveGetWeight(const UReasonablePlanningState* CurrentState) const;
	virtual float ReceiveGetWeight_Implementation(const UReasonablePlanningState* GivenState) const;
};
