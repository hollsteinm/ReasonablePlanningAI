// Troll Purse. All rights reserved.

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

public:
	virtual EStateQueryResult Query(const URpaiState* StateToQuery) const override;

	FORCEINLINE void SetSubQueries(TArray<URpaiComposerStateQuery*> NewSubQueries) { SubQueries = NewSubQueries; }
};
