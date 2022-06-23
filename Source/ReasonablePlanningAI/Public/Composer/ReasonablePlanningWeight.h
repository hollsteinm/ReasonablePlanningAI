// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/ReasonablePlanningTypes.h"
#include "Composer/ReasonablePlanningWeightBase.h"
#include "ReasonablePlanningWeight.generated.h"

/**
 * Object weight algorithm with added property reference to state key
 */
UCLASS(Abstract)
class REASONABLEPLANNINGAI_API UReasonablePlanningWeight : public UReasonablePlanningWeightBase
{
	GENERATED_BODY()

public:
	UReasonablePlanningWeight();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "RPAI")
	FStateKeyValueReference WeightStateKeyValueReference;

public:
	
	FORCEINLINE void SetWeightStateKeyAndType(FName KeyName, EStatePropertyType ExpectedValueType)
	{
		WeightStateKeyValueReference.StateKeyName = KeyName;
		WeightStateKeyValueReference.ExpectedValueType = ExpectedValueType;
	}
	
};
