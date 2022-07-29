// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ReasonablePlanningDistanceBase.generated.h"

class UReasonablePlanningState;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced)
class REASONABLEPLANNINGAI_API UReasonablePlanningDistanceBase : public UObject
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
	
};
