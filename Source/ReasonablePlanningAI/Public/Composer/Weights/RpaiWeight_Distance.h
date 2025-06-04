// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/RpaiComposerWeightBase.h"
#include "RpaiWeight_Distance.generated.h"

class URpaiComposerDistance;

/**
 * A weight adapter that uses a distance algorithm to determine the weight
 */
UCLASS()
class REASONABLEPLANNINGAI_API URpaiWeight_Distance : public URpaiComposerWeightBase
{
   GENERATED_BODY()
    
protected:
    UPROPERTY(EditAnywhere, Category="Rpai")
    URpaiComposerDistance* Distance;
    
    virtual float ReceiveExecutionWeight_Implementation(const URpaiState* GivenState) const override;
    
public:
    FORCEINLINE void SetDistance(URpaiComposerDistance* NewDistance) { Distance = NewDistance; }
   
};
