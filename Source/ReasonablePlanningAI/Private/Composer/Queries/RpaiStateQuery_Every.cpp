// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.


#include "Composer/Queries/RpaiStateQuery_Every.h"

EStateQueryResult URpaiStateQuery_Every::ReceiveQuery_Implementation(const URpaiState* StateToQuery) const
{
   for (const auto Query : SubQueries)
   {
      if (Query->Query(StateToQuery) != EStateQueryResult::Succeeded)
      {
         return EStateQueryResult::Failed;
      }
   }
   return EStateQueryResult::Succeeded;
}