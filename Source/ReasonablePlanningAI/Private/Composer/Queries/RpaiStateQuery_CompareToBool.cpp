// Troll Purse. All rights reserved.


#include "Composer/Queries/RpaiStateQuery_CompareToBool.h"
#include "Core/RpaiState.h"

EStateQueryResult URpaiStateQuery_CompareToBool::Query(const URpaiState* StateToQuery) const
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