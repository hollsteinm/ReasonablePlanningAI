// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/ReasonablePlanningStateQuery.h"
#include "Core/ReasonablePlanningTypes.h"
#include "StateQuery_CompareTo.generated.h"

UENUM(BlueprintType)
enum class EStateQueryCompareToOperation : uint8
{
    EqualTo,
    NotEqualTo,
    GreaterThan,
    GreaterThanOrEqualTo,
    LessThan,
    LessThanOrEqualTo,
};

/**
 *  Base class exposing members that does a query against a state using comparison operations
 */
UCLASS(Abstract)
class REASONABLEPLANNINGAI_API UStateQuery_CompareTo : public UReasonablePlanningStateQuery
{
	GENERATED_BODY()
	
protected:
    UPROPERTY(EditAnywhere, Category = "RPAI")
    FStateKeyValueReference QueriedState;
    
    UPROPERTY(EditAnywhere, Category = "RPAI")
    EStateQueryCompareToOperation ComparisonOperation;

public:
    FORCEINLINE void SetQueriedState(FName Key, EStatePropertyType ExpectedType) { QueriedState.StateKeyName = Key; QueriedState.ExpectedValueType = ExpectedType; }
    
    FORCEINLINE void SetComparisonOperation(EStateQueryCompareToOperation NewComparisonOperation) { ComparisonOperation = NewComparisonOperation; }
};

/**
 * Helpers using templates and specializations so that writing all the compare to operations require less work.
 */
template<typename T>
FORCEINLINE EStateQueryResult DoCompare(EStateQueryCompareToOperation Op, T LHS, T RHS)
{
    switch(Op)
    {
        case EStateQueryCompareToOperation::EqualTo:
            return LHS == RHS ? EStateQueryResult::Succeeded : EStateQueryResult::Failed;
        case EStateQueryCompareToOperation::NotEqualTo:
            return LHS != RHS ? EStateQueryResult::Succeeded : EStateQueryResult::Failed;
        case EStateQueryCompareToOperation::GreaterThan:
            return LHS > RHS ? EStateQueryResult::Succeeded : EStateQueryResult::Failed;
        case EStateQueryCompareToOperation::GreaterThanOrEqualTo:
            return LHS >= RHS ? EStateQueryResult::Succeeded : EStateQueryResult::Failed;
        case EStateQueryCompareToOperation::LessThan:
            return LHS < RHS ? EStateQueryResult::Succeeded : EStateQueryResult::Failed;
        case EStateQueryCompareToOperation::LessThanOrEqualTo:
            return LHS <= RHS ? EStateQueryResult::Succeeded : EStateQueryResult::Failed;
        default:
            return EStateQueryResult::Invalid;
    }
}

/**
 * Rotator specialization. Nothing considered for less than and greater than flavors of operations.
 */
template<>
FORCEINLINE EStateQueryResult DoCompare(EStateQueryCompareToOperation Op, FRotator LHS, FRotator RHS)
{
    switch(Op)
    {
        case EStateQueryCompareToOperation::EqualTo:
            return LHS == RHS ? EStateQueryResult::Succeeded : EStateQueryResult::Failed;
        case EStateQueryCompareToOperation::NotEqualTo:
            return LHS != RHS ? EStateQueryResult::Succeeded : EStateQueryResult::Failed;
        case EStateQueryCompareToOperation::GreaterThan:
        case EStateQueryCompareToOperation::GreaterThanOrEqualTo:
        case EStateQueryCompareToOperation::LessThan:
        case EStateQueryCompareToOperation::LessThanOrEqualTo:
        default:
            return EStateQueryResult::Invalid;
    }
}

/**
 * Vector specialization. We could consider comparing the magnitude of vectors for the less than and greater than operations.
 */
template<>
FORCEINLINE EStateQueryResult DoCompare(EStateQueryCompareToOperation Op, FVector LHS, FVector RHS)
{
    switch(Op)
    {
        case EStateQueryCompareToOperation::EqualTo:
            return LHS == RHS ? EStateQueryResult::Succeeded : EStateQueryResult::Failed;
        case EStateQueryCompareToOperation::NotEqualTo:
            return LHS != RHS ? EStateQueryResult::Succeeded : EStateQueryResult::Failed;
        case EStateQueryCompareToOperation::GreaterThan:
        case EStateQueryCompareToOperation::GreaterThanOrEqualTo:
        case EStateQueryCompareToOperation::LessThan:
        case EStateQueryCompareToOperation::LessThanOrEqualTo:
        default:
            return EStateQueryResult::Invalid;
    }
}
