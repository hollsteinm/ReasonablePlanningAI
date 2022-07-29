// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/ReasonablePlanningTypes.h"
#include "Composer/ReasonablePlanningDistanceBase.h"
#include "ReasonablePlanningDistance.generated.h"

/**
 * Algorithm class to determine the distance between two state values
 */
UCLASS(BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced)
class REASONABLEPLANNINGAI_API UReasonablePlanningDistance : public UReasonablePlanningDistanceBase
{
	GENERATED_BODY()

protected:

	/**
	* Left hand state key reference to use
	*/
	UPROPERTY(EditDefaultsOnly, Category = "RPAI", meta=(DisplayName="Left Hand Side State Reference Key"))
	FStateKeyValueReference LHS;

public:
	FORCEINLINE void SetLHS(FName Key, EStatePropertyType ExpectedType) { LHS.StateKeyName = Key; LHS.ExpectedValueType = ExpectedType; }
	
};
