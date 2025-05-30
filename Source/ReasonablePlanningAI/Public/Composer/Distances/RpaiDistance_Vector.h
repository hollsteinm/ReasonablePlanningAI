// Radaway Software LLC. 2025. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/RpaiComposerDistance.h"
#include "RpaiDistance_Vector.generated.h"

/**
 * Determines the distance between the state defined vector value and a defined vector. Will return the distance squared between the two vectors from the state value to the RHS property value.
 */
UCLASS()
class REASONABLEPLANNINGAI_API URpaiDistance_Vector : public URpaiComposerDistance
{
	GENERATED_BODY()
	
protected:
    virtual float ReceiveCalculateDistance_Implementation(const URpaiState* GivenState) const override;
    
    /**
    * Right hand value to use for comparison to the state value (LHS)
    */
    UPROPERTY(EditAnywhere, Category = "Rpai")
    FVector RHS;
    
public:
    FORCEINLINE void SetRHS(FVector NewRHS) { RHS = NewRHS; }
};
