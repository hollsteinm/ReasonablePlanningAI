// Troll Purse. All rights reserved.


#include "Composer/Queries/StateQuery_CompareToInteger.h"
#include "Core/ReasonablePlanningState.h"

EStateQueryResult UStateQuery_CompareToInteger::Query(const UReasonablePlanningState* StateToQuery) const
{
	if (QueriedState.ExpectedValueType != EStatePropertyType::Int)
	{
		return EStateQueryResult::Invalid;
	}

	int32 Value;
	if (!StateToQuery->GetInt(QueriedState.StateKeyName, Value))
	{
		return EStateQueryResult::Invalid;
	}
	return DoCompare(ComparisonOperation, Value, ValueToCompare);
}
