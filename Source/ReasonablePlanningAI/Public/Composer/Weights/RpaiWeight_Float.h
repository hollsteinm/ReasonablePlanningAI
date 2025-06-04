// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/RpaiComposerWeight.h"
#include "RpaiWeight_Float.generated.h"

/**
 * Returns the float value from state as the weight for the owning action
 */
UCLASS()
class REASONABLEPLANNINGAI_API URpaiWeight_Float : public URpaiComposerWeight
{
   GENERATED_BODY()
   
protected:
   virtual float ReceiveExecutionWeight_Implementation(const URpaiState* GivenState) const override;
};
