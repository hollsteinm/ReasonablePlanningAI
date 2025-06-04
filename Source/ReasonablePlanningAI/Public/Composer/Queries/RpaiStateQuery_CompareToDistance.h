// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/Queries/RpaiStateQuery_CompareTo.h"
#include "RpaiStateQuery_CompareToDistance.generated.h"

class URpaiComposerDistance;

/**
 * Compares the queried state value to the distance calculated by the distance algorithm applied to the same given state
 */
UCLASS()
class REASONABLEPLANNINGAI_API URpaiStateQuery_CompareToDistance : public URpaiStateQuery_CompareTo
{
   GENERATED_BODY()

protected:
    virtual EStateQueryResult ReceiveQuery_Implementation(const URpaiState* StateToQuery) const override;
   
    UPROPERTY(EditAnywhere,Category="Rpai")
    URpaiComposerDistance* Distance;

public:
   FORCEINLINE void SetComparisonValue(URpaiComposerDistance* NewDistance) { Distance = NewDistance; }
};
