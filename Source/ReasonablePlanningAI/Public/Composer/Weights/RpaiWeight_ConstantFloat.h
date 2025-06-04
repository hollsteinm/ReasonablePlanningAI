// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/RpaiComposerWeightBase.h"
#include "RpaiWeight_ConstantFloat.generated.h"

/**
 * 
 */
UCLASS()
class REASONABLEPLANNINGAI_API URpaiWeight_ConstantFloat : public URpaiComposerWeightBase
{
   GENERATED_BODY()

public:
   URpaiWeight_ConstantFloat();

   UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Rpai", meta = (ClampMin = "0.0", UIMin = "0.0"))
   float ConstantWeight;
   
protected:
   virtual float ReceiveExecutionWeight_Implementation(const URpaiState* GivenState) const override;
};
