// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/Queries/RpaiStateQuery_CompareTo.h"
#include "RpaiStateQuery_CompareToInteger.generated.h"

/**
 * Compares the state value as a int to the given int value
 */
UCLASS()
class REASONABLEPLANNINGAI_API URpaiStateQuery_CompareToInteger : public URpaiStateQuery_CompareTo
{
	GENERATED_BODY()
	
public:
	virtual EStateQueryResult Query(const URpaiState* StateToQuery) const override;

protected:
	UPROPERTY(EditAnywhere, Category = "Rpai")
	int32 ValueToCompare;

public:
	FORCEINLINE void SetComparisonValue(int32 NewValueToCompare) { ValueToCompare = NewValueToCompare; }
};
