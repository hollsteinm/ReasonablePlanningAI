// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RpaiComposerWeightBase.generated.h"

class URpaiState;

/**
 * Object representing a weight algorithm
 */
UCLASS(BlueprintType, Blueprintable, Abstract, EditInlineNew, DefaultToInstanced)
class REASONABLEPLANNINGAI_API URpaiComposerWeightBase : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Rpai")
	float ExecutionWeight(const URpaiState* GivenState) const;

protected:

	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "ResoanablePlanning")
	float ReceiveExecutionWeight(const URpaiState* GivenState) const;
	virtual float ReceiveExecutionWeight_Implementation(const URpaiState* GivenState) const;

	
};
