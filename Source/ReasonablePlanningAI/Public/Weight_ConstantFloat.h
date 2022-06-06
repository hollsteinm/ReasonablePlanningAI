// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ReasonablePlanningWeightBase.h"
#include "Weight_ConstantFloat.generated.h"

/**
 * 
 */
UCLASS()
class REASONABLEPLANNINGAI_API UWeight_ConstantFloat : public UReasonablePlanningWeightBase
{
	GENERATED_BODY()

public:
	UWeight_ConstantFloat();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ReasonablePlanningAI", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float ConstantWeight;
	
protected:
	virtual float ReceiveExecutionWeight_Implementation(const UReasonablePlanningState* GivenState) const override;
};
