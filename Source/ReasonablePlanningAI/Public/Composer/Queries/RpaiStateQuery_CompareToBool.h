// Radaway Software LLC. 2025. All rights reserved.

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

protected:
	virtual EStateQueryResult ReceiveQuery_Implementation(const URpaiState* StateToQuery) const override;
	
	UPROPERTY(EditAnywhere, Category = "Rpai")
	bool bValueToCompare;

public:
	FORCEINLINE void SetComparisonValue(bool bNewValueToCompare) { bValueToCompare = bNewValueToCompare; }
};
