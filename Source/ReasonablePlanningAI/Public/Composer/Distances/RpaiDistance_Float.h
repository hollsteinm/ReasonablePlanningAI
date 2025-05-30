// Radaway Software LLC. 2025. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/RpaiComposerDistance.h"
#include "RpaiDistance_Float.generated.h"

/**
 * Returns the difference of the LHS side float value to the RHS float value provided.
 */
UCLASS()
class REASONABLEPLANNINGAI_API URpaiDistance_Float : public URpaiComposerDistance
{
	GENERATED_BODY()

protected:
    virtual float ReceiveCalculateDistance_Implementation(const URpaiState* GivenState) const override;
    
    /**
    * Right hand value to use for comparison to the state value (LHS)
    */
    UPROPERTY(EditAnywhere, Category = "Rpai")
    float RHS;
    
public:
    FORCEINLINE void SetRHS(float NewRHS) { RHS = NewRHS; }
};
