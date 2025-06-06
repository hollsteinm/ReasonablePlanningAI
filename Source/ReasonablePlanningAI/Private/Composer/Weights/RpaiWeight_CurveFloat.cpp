// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.


#include "Composer/Weights/RpaiWeight_CurveFloat.h"
#include "Curves/CurveFloat.h"
#include "Core/RpaiState.h"

float URpaiWeight_CurveFloat::ReceiveExecutionWeight_Implementation(const URpaiState* GivenState) const
{
   float FloatInput = 0.f;
   int32 IntegerInput = 0;
   if (GivenState->GetFloat(WeightStateKeyValueReference.StateKeyName, FloatInput))
   {
      return WeightFloatCurve->GetFloatValue(FloatInput);
   }
   else if (GivenState->GetInt(WeightStateKeyValueReference.StateKeyName, IntegerInput))
   {
      return WeightFloatCurve->GetFloatValue(static_cast<float>(IntegerInput));
   }
   return 0.f;
}
