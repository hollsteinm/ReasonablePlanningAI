// Troll Purse. All rights reserved.


#include "Composer/Weights/Weight_Select.h"
#include "Composer/ReasonablePlanningStateQuery.h"

float UWeight_Select::ReceiveExecutionWeight_Implementation(const UReasonablePlanningState* GivenState) const
{
	check(GivenState != nullptr);
	check(Default != nullptr);

	for (const auto& Selection : Selections)
	{
		check(Selection.SelectionQuery != nullptr);
		check(Selection.SelectionWeight != nullptr);
		if (Selection.SelectionQuery->Query(GivenState) == EStateQueryResult::Succeeded)
		{
			return Selection.SelectionWeight->ExecutionWeight(GivenState);
		}
	}

	return Default->ExecutionWeight(GivenState);
}