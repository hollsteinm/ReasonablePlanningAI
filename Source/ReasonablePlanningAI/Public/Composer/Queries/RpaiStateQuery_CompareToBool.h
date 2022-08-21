// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/Queries/RpaiStateQuery_CompareTo.h"
#include "RpaiStateQuery_CompareToBool.generated.h"

/**
 * Compares the state value as a boolean to the given boolean value
 */
UCLASS()
class REASONABLEPLANNINGAI_API URpaiStateQuery_CompareToBool : public URpaiStateQuery_CompareTo
{
	GENERATED_BODY()

public:
	virtual EStateQueryResult Query(const URpaiState* StateToQuery) const override;
	
protected:
	UPROPERTY(EditAnywhere, Category = "Rpai")
	bool bValueToCompare;

public:
	FORCEINLINE void SetComparisonValue(bool bNewValueToCompare) { bValueToCompare = bNewValueToCompare; }
};
