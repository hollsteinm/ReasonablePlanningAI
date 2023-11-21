// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/Queries/RpaiStateQuery_CompareTo.h"
#include "Core/RpaiTypes.h"
#include "RpaiStateQuery_CompareToStateValue.generated.h"

/**
 * Compares the queried state value to a state value within the same state. Returns success if the comparison operation holds true.
 */
UCLASS()
class REASONABLEPLANNINGAI_API URpaiStateQuery_CompareToStateValue : public URpaiStateQuery_CompareTo
{
	GENERATED_BODY()
    
protected:
    UPROPERTY(EditAnywhere, Category = "Rpai")
    FStateKeyValueReference ComparedToState;
    
    virtual EStateQueryResult ReceiveQuery_Implementation(const URpaiState* StateToQuery) const override;
public:

    FORCEINLINE void SetComparedToState(FName Key, EStatePropertyType ExpectedType) { ComparedToState.StateKeyName = Key; ComparedToState.ExpectedValueType = ExpectedType; }
	
};
