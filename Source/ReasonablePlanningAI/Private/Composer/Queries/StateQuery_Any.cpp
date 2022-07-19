// Troll Purse. All rights reserved.


#include "Composer/Queries/StateQuery_Any.h"

EStateQueryResult UStateQuery_Any::Query(const UReasonablePlanningState* StateToQuery) const
{
	//What is any of nothing? Every of nothing is true for us right now, but is any of nothing success?
	for (const auto Query : SubQueries)
	{
		if (Query->Query(StateToQuery) == EStateQueryResult::Succeeded)
		{
			return EStateQueryResult::Succeeded;
		}
	}
	return EStateQueryResult::Failed;
}