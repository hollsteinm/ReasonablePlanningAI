// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/RpaiComposerDistance.h"
#include "RpaiDistance_Rotator.generated.h"

/**
 * Determines the distance between to rotators, with the state value being the left hand side value and the property RHS being the right hand side. Distance returned is the Manhattan distance.
 */
UCLASS()
class REASONABLEPLANNINGAI_API URpaiDistance_Rotator : public URpaiComposerDistance
{
   GENERATED_BODY()
   
protected:
    virtual float ReceiveCalculateDistance_Implementation(const URpaiState* GivenState) const override;
    
    /**
    * Right hand value to use for comparison to the state value (LHS)
    */
    UPROPERTY(EditAnywhere, Category = "Rpai")
    FRotator RHS;
    
public:
    FORCEINLINE void SetRHS(FRotator NewRHS) { RHS = NewRHS; }
};
