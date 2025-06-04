// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.

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

protected:
   virtual EStateQueryResult ReceiveQuery_Implementation(const URpaiState* StateToQuery) const override;

   UPROPERTY(EditAnywhere, Category = "Rpai")
   float ValueToCompare;

public:
   FORCEINLINE void SetComparisonValue(float NewValueToCompare) { ValueToCompare = NewValueToCompare; }
};
