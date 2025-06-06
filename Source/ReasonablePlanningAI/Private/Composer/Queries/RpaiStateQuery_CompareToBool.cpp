// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.


#include "Composer/Queries/RpaiStateQuery_CompareToBool.h"
#include "Core/RpaiState.h"

EStateQueryResult URpaiStateQuery_CompareToBool::ReceiveQuery_Implementation(const URpaiState* StateToQuery) const
{
   if (QueriedState.ExpectedValueType != EStatePropertyType::Bool)
   {
      return EStateQueryResult::Invalid;
   }

   bool bValue;
   if (!StateToQuery->GetBool(QueriedState.StateKeyName, bValue))
   {
      return EStateQueryResult::Invalid;
   }
   return DoCompare(ComparisonOperation, bValue, bValueToCompare);
}