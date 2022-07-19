// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/ReasonablePlanningStateQuery.h"
#include "StateQuery_Every.generated.h"

/**
 * Evaluatates a set of sub-queries. All sub-queries must succeed.
 */
UCLASS()
class REASONABLEPLANNINGAI_API UStateQuery_Every : public UReasonablePlanningStateQuery
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "RPAI", Instanced)
	TArray<UReasonablePlanningStateQuery*> SubQueries;
	
public:
	virtual EStateQueryResult Query(const UReasonablePlanningState* StateToQuery) const override;

	FORCEINLINE void SetSubQueries(TArray<UReasonablePlanningStateQuery*> NewSubQueries) { SubQueries = NewSubQueries; }
};
