// Troll Purse. All rights reserved.


#include "Composer/Queries/StateQuery_CompareToFloat.h"
#include "Core/ReasonablePlanningState.h"

EStateQueryResult UStateQuery_CompareToFloat::Query(const UReasonablePlanningState* StateToQuery) const
{
	if (QueriedState.ExpectedValueType != EStatePropertyType::Float)
	{
		return EStateQueryResult::Invalid;
	}

	float Value;
	if (!StateToQuery->GetFloat(QueriedState.StateKeyName, Value))
	{
		return EStateQueryResult::Invalid;
	}
	return DoCompare(ComparisonOperation, Value, ValueToCompare);
}
