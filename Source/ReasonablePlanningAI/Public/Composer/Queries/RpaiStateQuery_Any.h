// Radaway Software LLC. 2025. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/RpaiComposerStateQuery.h"
#include "RpaiStateQuery_Any.generated.h"

/**
 * Evaluates a set of sub-queries. At least one subquery must return success for this to succeed.
 */
UCLASS()
class REASONABLEPLANNINGAI_API URpaiStateQuery_Any : public URpaiComposerStateQuery
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, Category = "Rpai", Instanced)
		TArray<URpaiComposerStateQuery*> SubQueries;

	virtual EStateQueryResult ReceiveQuery_Implementation(const URpaiState* StateToQuery) const override;

public:
	FORCEINLINE void SetSubQueries(TArray<URpaiComposerStateQuery*> NewSubQueries) { SubQueries = NewSubQueries; }
};
