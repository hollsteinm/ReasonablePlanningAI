// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.


#include "Composer/Distances/RpaiDistance_Select.h"
#include "Composer/RpaiComposerStateQuery.h"

float URpaiDistance_Select::ReceiveCalculateDistance_Implementation(const URpaiState* GivenState) const
{
   check(GivenState != nullptr);
   check(Default != nullptr);

   for (const auto& Selection : Selections)
   {
      check(Selection.SelectionQuery != nullptr);
      check(Selection.SelectionDistance != nullptr);
      if (Selection.SelectionQuery->Query(GivenState) == EStateQueryResult::Succeeded)
      {
         return Selection.SelectionDistance->CalculateDistance(GivenState);
      }
   }

   return Default->CalculateDistance(GivenState);
}