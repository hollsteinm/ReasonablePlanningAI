// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/RpaiTypes.h"
#include "Composer/RpaiComposerWeightBase.h"
#include "RpaiComposerWeight.generated.h"

/**
 * Object weight algorithm with added property reference to state key
 */
UCLASS(Abstract)
class REASONABLEPLANNINGAI_API URpaiComposerWeight : public URpaiComposerWeightBase
{
	GENERATED_BODY()

public:
	URpaiComposerWeight();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Rpai")
	FStateKeyValueReference WeightStateKeyValueReference;

public:
	
	FORCEINLINE void SetWeightStateKeyAndType(FName KeyName, EStatePropertyType ExpectedValueType)
	{
		WeightStateKeyValueReference.StateKeyName = KeyName;
		WeightStateKeyValueReference.ExpectedValueType = ExpectedValueType;
	}
	
};
