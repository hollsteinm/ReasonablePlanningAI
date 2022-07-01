// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/Queries/StateQuery_CompareTo.h"
#include "StateQuery_CompareToFloat.generated.h"

/**
 * Compares the state value as a float to the given float value
 */
UCLASS()
class REASONABLEPLANNINGAI_API UStateQuery_CompareToFloat : public UStateQuery_CompareTo
{
	GENERATED_BODY()

public:
	virtual EStateQueryResult Query(const UReasonablePlanningState* StateToQuery) const override;

protected:
	UPROPERTY(EditAnywhere, Category = "RPAI")
	float ValueToCompare;

public:
	FORCEINLINE void SetComparisonValue(float NewValueToCompare) { ValueToCompare = NewValueToCompare; }
};
