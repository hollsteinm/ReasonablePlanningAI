// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.


#include "Composer/Weights/RpaiWeight_Select.h"
#include "Composer/RpaiComposerStateQuery.h"

float URpaiWeight_Select::ReceiveExecutionWeight_Implementation(const URpaiState* GivenState) const
{
   check(GivenState != nullptr);
   check(Default != nullptr);

   for (const auto& Selection : Selections)
   {
      check(Selection.SelectionQuery != nullptr);
      check(Selection.SelectionWeight != nullptr);
      if (Selection.SelectionQuery->Query(GivenState) == EStateQueryResult::Succeeded)
      {
         return Selection.SelectionWeight->ExecutionWeight(GivenState);
      }
   }

   return Default->ExecutionWeight(GivenState);
}