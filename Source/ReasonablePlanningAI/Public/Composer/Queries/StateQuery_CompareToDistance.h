// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/Queries/StateQuery_CompareTo.h"
#include "StateQuery_CompareToDistance.generated.h"

/**
 * Compares the queried state value to the distance calculated by the distance algorithm applied to the same given state
 */
UCLASS()
class REASONABLEPLANNINGAI_API UStateQuery_CompareToDistance : public UStateQuery_CompareTo
{
	GENERATED_BODY()
public:
    virtual EStateQueryResult Query(const UReasonablePlanningState* StateToQuery) const override;
	
protected:
    UPROPERTY(EditAnywhere,Category="RPAI")
    UReasonablePlanningDistance* Distance;

public:
	FORCEINLINE void SetComparisonValue(UReasonablePlanningDistance* NewDistance) { Distance = NewDistance; }
};
