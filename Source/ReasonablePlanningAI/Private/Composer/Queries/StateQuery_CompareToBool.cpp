// Troll Purse. All rights reserved.


#include "Composer/Queries/StateQuery_CompareToBool.h"
#include "Core/ReasonablePlanningState.h"

EStateQueryResult UStateQuery_CompareToBool::Query(const UReasonablePlanningState* StateToQuery) const
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