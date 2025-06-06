// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.


#include "Composer/Queries/RpaiStateQuery_Any.h"

EStateQueryResult URpaiStateQuery_Any::ReceiveQuery_Implementation(const URpaiState* StateToQuery) const
{
   //What is any of nothing? Every of nothing is true for us right now, but is any of nothing success?
   for (const auto Query : SubQueries)
   {
      if (Query->Query(StateToQuery) == EStateQueryResult::Succeeded)
      {
         return EStateQueryResult::Succeeded;
      }
   }
   return EStateQueryResult::Failed;
}