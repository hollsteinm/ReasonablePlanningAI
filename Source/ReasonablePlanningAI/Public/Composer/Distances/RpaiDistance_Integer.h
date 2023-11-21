// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/RpaiComposerDistance.h"
#include "RpaiDistance_Integer.generated.h"

/**
 * Determines the distance from the float state value to an integer by using the difference of the two values.
 */
UCLASS()
class REASONABLEPLANNINGAI_API URpaiDistance_Integer : public URpaiComposerDistance
{
	GENERATED_BODY()
	
protected:
    virtual float ReceiveCalculateDistance_Implementation(const URpaiState* GivenState) const override;
    
    /**
    * Right hand value to use for comparison to the state value (LHS)
    */
    UPROPERTY(EditAnywhere, Category = "Rpai")
    int32 RHS;
    
public:
    FORCEINLINE void SetRHS(int32 NewRHS) { RHS = NewRHS; }
};
