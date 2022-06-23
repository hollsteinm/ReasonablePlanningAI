// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/Queries/StateQuery_CompareTo.h"
#include "StateQuery_CompareToBool.generated.h"

/**
 * Compares the state value as a boolean to the given boolean value
 */
UCLASS()
class REASONABLEPLANNINGAI_API UStateQuery_CompareToBool : public UStateQuery_CompareTo
{
	GENERATED_BODY()

public:
	virtual EStateQueryResult Query(const UReasonablePlanningState* StateToQuery) const override;
	
protected:
	UPROPERTY(EditAnywhere, Category = "RPAI")
	bool bValueToCompare;
};
