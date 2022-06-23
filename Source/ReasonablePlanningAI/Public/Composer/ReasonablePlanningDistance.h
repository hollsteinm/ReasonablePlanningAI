// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/ReasonablePlanningTypes.h"
#include "ReasonablePlanningDistance.generated.h"

class UReasonablePlanningState;

/**
 * Algorithm class to determine the distance between two state values
 */
UCLASS(BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced)
class REASONABLEPLANNINGAI_API UReasonablePlanningDistance : public UObject
{
	GENERATED_BODY()

public:
	/**
	* Public interface to determine distance between two compared values in a give ReasonablePlanningState
	*/
	UFUNCTION(BlueprintPure, Category = "RPAI")
	float CalculateDistance(const UReasonablePlanningState* GivenState) const;

protected:
	/**
	* Override to implement CalculateDistance
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "RPAI")
	float ReceiveCalculateDistance(const UReasonablePlanningState* GivenState) const;
	virtual float ReceiveCalculateDistance_Implementation(const UReasonablePlanningState* GivenState) const;

	/**
	* Left hand state key reference to use
	*/
	UPROPERTY(EditDefaultsOnly, Category = "RPAI", meta=(DisplayName="Left Hand Side State Reference Key"))
	FStateKeyValueReference LHS;

	/**
	* Right hand state key reference to use
	*/
	UPROPERTY(EditDefaultsOnly, Category = "RPAI", meta = (DisplayName = "Right Hand Side State Reference Key"))
	FStateKeyValueReference RHS;

public:
	FORCEINLINE void SetLHS(FName Key, EStatePropertyType ExpectedType) { LHS.StateKeyName = Key; LHS.ExpectedValueType = ExpectedType; }
	FORCEINLINE void SetRHS(FName Key, EStatePropertyType ExpectedType) { RHS.StateKeyName = Key; RHS.ExpectedValueType = ExpectedType; }
	
};
