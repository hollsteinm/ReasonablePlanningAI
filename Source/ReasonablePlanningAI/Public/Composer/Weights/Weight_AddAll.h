// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/ReasonablePlanningWeightBase.h"
#include "Weight_AddAll.generated.h"

/**
 * Returns the summation of all the SubWeights.
 */
UCLASS()
class REASONABLEPLANNINGAI_API UWeight_AddAll : public UReasonablePlanningWeightBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere,Category="RPAI",Instanced)
	TArray<UReasonablePlanningWeightBase*> SubWeights;

	virtual float ReceiveExecutionWeight_Implementation(const UReasonablePlanningState* GivenState) const override;

public:
	FORCEINLINE void SetSubWeights(TArray<UReasonablePlanningWeightBase*>&& NewSubWeights) { SubWeights = NewSubWeights; }
};
