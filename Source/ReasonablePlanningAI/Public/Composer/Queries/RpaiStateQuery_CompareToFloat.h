// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/Queries/RpaiStateQuery_CompareTo.h"
#include "RpaiStateQuery_CompareToFloat.generated.h"

/**
 * Compares the state value as a float to the given float value
 */
UCLASS()
class REASONABLEPLANNINGAI_API URpaiStateQuery_CompareToFloat : public URpaiStateQuery_CompareTo
{
	GENERATED_BODY()

public:
	virtual EStateQueryResult Query(const URpaiState* StateToQuery) const override;

protected:
	UPROPERTY(EditAnywhere, Category = "Rpai")
	float ValueToCompare;

public:
	FORCEINLINE void SetComparisonValue(float NewValueToCompare) { ValueToCompare = NewValueToCompare; }
};
