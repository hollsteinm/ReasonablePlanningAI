// Troll Purse. All rights reserved.


#include "Composer/Queries/StateQuery_CompareToFloat.h"
#include "Core/ReasonablePlanningState.h"

EStateQueryResult UStateQuery_CompareToFloat::Query(const UReasonablePlanningState* StateToQuery) const
{
	if (QueriedState.ExpectedValueType != EStatePropertyType::Float || QueriedState.ExpectedValueType != EStatePropertyType::Int)
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
