// Radaway Software LLC. 2025. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/RpaiComposerDistance.h"
#include "RpaiDistance_Bool.generated.h"

/**
 * Compares a state boolean value to the given boolean value, returns 1.0 if they are the same, 0.0 if they are different.
 */
UCLASS()
class REASONABLEPLANNINGAI_API URpaiDistance_Bool : public URpaiComposerDistance
{
	GENERATED_BODY()

protected:
    virtual float ReceiveCalculateDistance_Implementation(const URpaiState* GivenState) const override;
    
    /**
    * Right hand value to use for comparison to the state value (LHS)
    */
    UPROPERTY(EditAnywhere, Category = "Rpai")
    bool bRHS;
    
public:
    FORCEINLINE void SetRHS(bool bNewRHS) { bRHS = bNewRHS; }
};
