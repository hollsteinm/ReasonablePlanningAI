// Troll Purse. All rights reserved.


#include "Composer/Queries/RpaiStateQuery_CompareToInteger.h"
#include "Core/RpaiState.h"

EStateQueryResult URpaiStateQuery_CompareToInteger::ReceiveQuery_Implementation(const URpaiState* StateToQuery) const
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
