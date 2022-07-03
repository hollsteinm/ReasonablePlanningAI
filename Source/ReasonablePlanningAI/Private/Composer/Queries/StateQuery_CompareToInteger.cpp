// Troll Purse. All rights reserved.


#include "Composer/Queries/StateQuery_CompareToInteger.h"
#include "Core/ReasonablePlanningState.h"

EStateQueryResult UStateQuery_CompareToInteger::Query(const UReasonablePlanningState* StateToQuery) const
{
	if (QueriedState.ExpectedValueType != EStatePropertyType::Int && QueriedState.ExpectedValueType != EStatePropertyType::Float)
	{
		return EStateQueryResult::Invalid;
	}

	int32 Value;
	float fValue;
	if (StateToQuery->GetValueOfType(QueriedState.StateKeyName, Value))
	{
		return DoCompare(ComparisonOperation, Value, ValueToCompare);
	}
	else if (StateToQuery->GetValueOfType(QueriedState.StateKeyName, fValue))
	{
		return DoCompare(ComparisonOperation, FMath::FloorToInt(fValue), ValueToCompare);
	}
	else
	{
		return EStateQueryResult::Invalid;
	}
}
