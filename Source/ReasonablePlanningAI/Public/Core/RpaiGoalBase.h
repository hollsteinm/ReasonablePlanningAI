// Radaway Software LLC. 2025. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RpaiGoalBase.generated.h"

class URpaiState;

/**
 * A goal is the system that determines what the overall desired state is
 * and calculates how much work it will take to get from a given current state
 * to a given desired state. A goal will also have reasoning components to it.
 * These components are a category weight and a dynamic weight function. A goal
 * determines utility - how useful a given goal is in a given situation.
 */
UCLASS(BlueprintType, Blueprintable, Abstract, EditInlineNew, DefaultToInstanced)
class REASONABLEPLANNINGAI_API URpaiGoalBase : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, BlueprintGetter = GetCategory, Category = "Rpai")
	int32 Category;

public:
	URpaiGoalBase();

	/**
	* Simple binary function to indicate whether a desired state for this goal
	* has been acheived. As it is a pure function, this can be used to determine
	* a real state or a desired state.
	*/
	UFUNCTION(BlueprintPure, Category = "Rpai")
	bool IsInDesiredState(const URpaiState* GivenState) const;

	/**
	* Determines how much effort it will take to get to the given desired state.
	*/
	UFUNCTION(BlueprintPure, Category = "Rpai")
	float GetDistanceToDesiredState(const URpaiState* GivenState) const;

	/**
	* Determines whether the utility of this goal is even possible or relevant given
	* the current situtation/state.
	*/
	UFUNCTION(BlueprintPure, Category = "Rpai")
	bool IsApplicable(const URpaiState* GivenState) const;

	/**
	* Returns some arbitrary grouping value to use for reasoning. Should match
	* the value used by other goals to indicate importance, where lower values
	* are preferred goals.
	*/
	UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Rpai")
	FORCEINLINE int32 GetCategory() const { return Category; }

	/**
	* A more dynamic function used for reasoning, this is a weight to indicate
	* the desirablility, or utility, of the goal based on the current state
	* given to the goal.
	*/
	UFUNCTION(BlueprintPure, Category = "Rpai")
	float GetWeight(const URpaiState* GivenState) const;
	
protected:
	/**
	* Override this to implement IsInDesiredState
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Rpai")
	bool ReceiveIsInDesiredState(const URpaiState* CurrentState) const;
	virtual bool ReceiveIsInDesiredState_Implementation(const URpaiState* GivenState) const;

	/**
	* Override this to implement GetDistanceToDesiredState. Lower distances indicate less work
	* to proceed to the desired state.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Rpai")
	float ReceiveGetDistanceToDesiredState(const URpaiState* CurrentState) const;
	virtual float ReceiveGetDistanceToDesiredState_Implementation(const URpaiState* GivenState) const;

	/**
	* Override this to implement IsApplicable
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Rpai")
	bool ReceiveIsApplicable(const URpaiState* CurrentState) const;
	virtual bool ReceiveIsApplicable_Implementation(const URpaiState* GivenState) const;

	/**
	* Override this to implement GetWeight. Higher weights indicate more utility (more desired to execute).
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Rpai")
	float ReceiveGetWeight(const URpaiState* CurrentState) const;
	virtual float ReceiveGetWeight_Implementation(const URpaiState* GivenState) const;
    
private:
    UPROPERTY(EditAnywhere, Category = "Rpai")
    FString GoalName;
    
public:
    UFUNCTION(BlueprintPure, Category = "Rpai")
    FString GetGoalName() const;

	FORCEINLINE void SetCategory(int32 NewCategory) { Category = NewCategory; }
};
