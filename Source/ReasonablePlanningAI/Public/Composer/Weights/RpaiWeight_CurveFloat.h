// Radaway Software LLC. 2025. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/RpaiComposerWeight.h"
#include "RpaiWeight_CurveFloat.generated.h"

/**
 * A weight that returns a float value based on a given curve and the supplied state property as input
 */
UCLASS()
class REASONABLEPLANNINGAI_API URpaiWeight_CurveFloat : public URpaiComposerWeight
{
   GENERATED_BODY()
   
protected:
   virtual float ReceiveExecutionWeight_Implementation(const URpaiState* GivenState) const override;

   UPROPERTY(EditAnywhere, Category = "Rpai")
   class UCurveFloat* WeightFloatCurve;

public:
   //Not UFUNCTION because this is for C++ setup and we want to keep the data above focused on game design editability.
   FORCEINLINE void SetCurve(UCurveFloat* NewCurve) { WeightFloatCurve = NewCurve; }
   FORCEINLINE UCurveFloat* GetCurve() const { return WeightFloatCurve; }
};
