// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Composer/RpaiComposerDistanceBase.h"
#include "RpaiDistance_CurveFloat.generated.h"

/**
 * Applies a curve to some given distance
 */
UCLASS()
class REASONABLEPLANNINGAI_API URpaiDistance_CurveFloat : public URpaiComposerDistanceBase
{
   GENERATED_BODY ()
protected:
   virtual float ReceiveCalculateDistance_Implementation(const URpaiState* GivenState) const override;

   UPROPERTY (EditAnywhere, Category = "Rpai");
   class UCurveFloat* DistanceFloatCurve;

   UPROPERTY (EditAnywhere, Category = "Rpai");
   URpaiComposerDistanceBase* Distance;

public:
   //Not UFUNCTION because this is for C++ setup and we want to keep the data above focused on game design editability.
   FORCEINLINE void SetCurve(UCurveFloat* NewCurve) { DistanceFloatCurve = NewCurve; }
   FORCEINLINE UCurveFloat* GetCurve() const { return DistanceFloatCurve; }

   FORCEINLINE void SetDistance(URpaiComposerDistanceBase* NewDistance) { Distance = NewDistance; }
   FORCEINLINE URpaiComposerDistanceBase* GetDistance() const { return Distance; }
};
