// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/Queries/StateQuery_CompareTo.h"
#include "Core/ReasonablePlanningTypes.h"
#include "StateQuery_CompareToStateValue.generated.h"

/**
 * Compares the queried state value to a state value within the same state. Returns success if the comparison operation holds true.
 */
UCLASS()
class REASONABLEPLANNINGAI_API UStateQuery_CompareToStateValue : public UStateQuery_CompareTo
{
	GENERATED_BODY()
    
protected:
    UPROPERTY(EditAnywhere, Category = "RPAI")
    FStateKeyValueReference ComparedToState;
    
public:
    virtual EStateQueryResult Query(const UReasonablePlanningState* StateToQuery) const override;

    FORCEINLINE void SetComparedToState(FName Key, EStatePropertyType ExpectedType) { ComparedToState.StateKeyName = Key; ComparedToState.ExpectedValueType = ExpectedType; }
	
};
