// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/ReasonablePlanningStateQuery.h"
#include "Composer/Queries/StateQuery_CompareTo.h"
#include "StateQuery_CompareDistanceFloat.generated.h"

class UReasonablePlanningDistance;

/**
 * Compares Distance calculated from state to the defined float value
 */
UCLASS()
class REASONABLEPLANNINGAI_API UStateQuery_CompareDistanceFloat : public UReasonablePlanningStateQuery
{
	GENERATED_BODY()
    
public:
    virtual EStateQueryResult Query(const UReasonablePlanningState* StateToQuery) const override;
    
protected:
    UPROPERTY(EditAnywhere, Category = "RPAI")
    EStateQueryCompareToOperation ComparisonOperation;

    UPROPERTY(EditAnywhere, Category="RPAI")
    UReasonablePlanningDistance* Distance;
    
    UPROPERTY(EditAnywhere, Category="RPAI")
    float RHS;
    
public:
    FORCEINLINE void SetDistance(UReasonablePlanningDistance* NewDistance) { Distance = NewDistance; }
    FORCEINLINE void SetRHS(float NewRHS) { RHS = NewRHS; }
    FORCEINLINE void SetComparisonOperation(EStateQueryCompareToOperation NewComparisonOperation) { ComparisonOperation = NewComparisonOperation; }
};
