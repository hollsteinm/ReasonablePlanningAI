// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ReasonablePlanningStateQuery.generated.h"

class UReasonablePlanningState;

UENUM(BlueprintType)
enum class EStateQueryResult : uint8
{
    Invalid,
    Succeeded,
    Failed
};

/**
 * Given configuration of a state property to compare to, return whether the conditions are satisfied.
 */
UCLASS(Abstract, BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced)
class REASONABLEPLANNINGAI_API UReasonablePlanningStateQuery : public UObject
{
	GENERATED_BODY()
	
public:
    /**
     * Implement this function to define your query operation. Return EStateQueryResult::Succeeded to indicate the query conditions
     * are satisfied. use EStateQueryResult::Failed to indicate that the query conditions were not satisifed. Use EStateQueryResult::Failed
     * for instances of invalid state properties, missing state values, or other such conditions.
     */
    UFUNCTION(BlueprintPure, Category = "RPAI")
    virtual EStateQueryResult Query(const UReasonablePlanningState* StateToQuery) const PURE_VIRTUAL(UReasonablePlanningStateQuery::Query, return EStateQueryResult::Invalid;);
    
};
