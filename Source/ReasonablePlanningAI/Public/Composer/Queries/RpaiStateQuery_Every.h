// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/RpaiComposerStateQuery.h"
#include "RpaiStateQuery_Every.generated.h"

/**
 * Evaluatates a set of sub-queries. All sub-queries must succeed.
 */
UCLASS()
class REASONABLEPLANNINGAI_API URpaiStateQuery_Every : public URpaiComposerStateQuery
{
   GENERATED_BODY()

protected:
   UPROPERTY(EditAnywhere, Category = "Rpai", Instanced)
   TArray<URpaiComposerStateQuery*> SubQueries;
   
   virtual EStateQueryResult ReceiveQuery_Implementation(const URpaiState* StateToQuery) const override;

public:
   FORCEINLINE void SetSubQueries(TArray<URpaiComposerStateQuery*>&& NewSubQueries) { SubQueries = NewSubQueries; }
};
