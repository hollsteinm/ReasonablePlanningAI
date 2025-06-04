// Radaway Software LLC. 2025. All rights reserved.


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