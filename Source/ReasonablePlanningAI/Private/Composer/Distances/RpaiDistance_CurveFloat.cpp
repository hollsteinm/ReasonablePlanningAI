// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.


#include "Composer/Distances/RpaiDistance_CurveFloat.h"
#include "Curves/CurveFloat.h"
#include "Core/RpaiState.h"

float URpaiDistance_CurveFloat::ReceiveCalculateDistance_Implementation(const URpaiState* GivenState) const
{
   if (Distance == nullptr || DistanceFloatCurve == nullptr)
   {
      return Super::ReceiveCalculateDistance_Implementation(GivenState);
   }

   float DistanceResult = Distance->CalculateDistance(GivenState);
   return DistanceFloatCurve->GetFloatValue(DistanceResult);
}