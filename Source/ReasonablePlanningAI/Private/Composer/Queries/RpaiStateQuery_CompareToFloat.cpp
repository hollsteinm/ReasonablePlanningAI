// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.


#include "Composer/Queries/RpaiStateQuery_CompareToFloat.h"
#include "Core/RpaiState.h"

EStateQueryResult URpaiStateQuery_CompareToFloat::ReceiveQuery_Implementation(const URpaiState* StateToQuery) const
{
   if (QueriedState.ExpectedValueType != EStatePropertyType::Float && QueriedState.ExpectedValueType != EStatePropertyType::Int)
   {
      return EStateQueryResult::Invalid;
   }

   float Value;
   int32 IntValue;
   if (StateToQuery->GetFloat(QueriedState.StateKeyName, Value))
   {
      return DoCompare(ComparisonOperation, Value, ValueToCompare);
   }
   else if(StateToQuery->GetInt(QueriedState.StateKeyName, IntValue))
   {
      return DoCompare(ComparisonOperation, float(IntValue), ValueToCompare);
   }
   else
   {
      return EStateQueryResult::Invalid;
   }
}
