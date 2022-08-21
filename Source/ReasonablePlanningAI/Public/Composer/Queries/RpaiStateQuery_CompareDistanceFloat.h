// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/RpaiComposerStateQuery.h"
#include "Composer/Queries/RpaiStateQuery_CompareTo.h"
#include "RpaiStateQuery_CompareDistanceFloat.generated.h"

class URpaiComposerDistance;

/**
 * Compares Distance calculated from state to the defined float value
 */
UCLASS()
class REASONABLEPLANNINGAI_API URpaiStateQuery_CompareDistanceFloat : public URpaiComposerStateQuery
{
	GENERATED_BODY()
    
public:
    virtual EStateQueryResult Query(const URpaiState* StateToQuery) const override;
    
protected:
    UPROPERTY(EditAnywhere, Category = "Rpai")
    EStateQueryCompareToOperation ComparisonOperation;

    UPROPERTY(EditAnywhere, Category="Rpai")
    URpaiComposerDistance* Distance;
    
    UPROPERTY(EditAnywhere, Category="Rpai")
    float RHS;
    
public:
    FORCEINLINE void SetDistance(URpaiComposerDistance* NewDistance) { Distance = NewDistance; }
    FORCEINLINE void SetRHS(float NewRHS) { RHS = NewRHS; }
    FORCEINLINE void SetComparisonOperation(EStateQueryCompareToOperation NewComparisonOperation) { ComparisonOperation = NewComparisonOperation; }
};
