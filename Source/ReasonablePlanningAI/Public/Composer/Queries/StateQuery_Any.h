// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/ReasonablePlanningStateQuery.h"
#include "StateQuery_Any.generated.h"

/**
 * Evaluates a set of sub-queries. At least one subquery must return success for this to succeed.
 */
UCLASS()
class REASONABLEPLANNINGAI_API UStateQuery_Any : public UReasonablePlanningStateQuery
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, Category = "RPAI", Instanced)
		TArray<UReasonablePlanningStateQuery*> SubQueries;

public:
	virtual EStateQueryResult Query(const UReasonablePlanningState* StateToQuery) const override;

	FORCEINLINE void SetSubQueries(TArray<UReasonablePlanningStateQuery*> NewSubQueries) { SubQueries = NewSubQueries; }
};
