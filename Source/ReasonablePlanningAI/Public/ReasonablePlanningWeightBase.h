// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ReasonablePlanningWeightBase.generated.h"

class UReasonablePlanningState;

/**
 * Object representing a weight algorithm
 */
UCLASS(BlueprintType, Blueprintable, Abstract, EditInlineNew, DefaultToInstanced)
class REASONABLEPLANNINGAI_API UReasonablePlanningWeightBase : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "ReasonablePlanningAI")
	float ExecutionWeight(const UReasonablePlanningState* GivenState) const;

protected:

	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "ResoanablePlanning")
	float ReceiveExecutionWeight(const UReasonablePlanningState* GivenState) const;
	virtual float ReceiveExecutionWeight_Implementation(const UReasonablePlanningState* GivenState) const;

	
};
