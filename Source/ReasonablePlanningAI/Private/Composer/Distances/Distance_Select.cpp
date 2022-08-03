// Troll Purse. All rights reserved.


#include "Composer/Distances/Distance_Select.h"
#include "Composer/ReasonablePlanningStateQuery.h"

float UDistance_Select::ReceiveCalculateDistance_Implementation(const UReasonablePlanningState* GivenState) const
{
	check(GivenState != nullptr);
	check(Default != nullptr);

	for (const auto& Selection : Selections)
	{
		check(Selection.SelectionQuery != nullptr);
		check(Selection.SelectionDistance != nullptr);
		if (Selection.SelectionQuery->Query(GivenState) == EStateQueryResult::Succeeded)
		{
			return Selection.SelectionDistance->CalculateDistance(GivenState);
		}
	}

	return Default->CalculateDistance(GivenState);
}