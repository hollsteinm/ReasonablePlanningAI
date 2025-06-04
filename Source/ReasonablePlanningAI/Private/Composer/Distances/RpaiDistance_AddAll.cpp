// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.


#include "Composer/Distances/RpaiDistance_AddAll.h"

float URpaiDistance_AddAll::ReceiveCalculateDistance_Implementation(const URpaiState* GivenState) const
{
   float Result = 0.f;
   for (const auto Distance : SubDistances)
   {
      Result += Distance->CalculateDistance(GivenState);
   }
   return Result;
}