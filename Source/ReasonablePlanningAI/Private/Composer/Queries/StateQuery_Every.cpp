// Troll Purse. All rights reserved.


#include "Composer/Queries/StateQuery_Every.h"

EStateQueryResult UStateQuery_Every::Query(const UReasonablePlanningState* StateToQuery) const
{
	for (const auto Query : SubQueries)
	{
		if (Query->Query(StateToQuery) != EStateQueryResult::Succeeded)
		{
			return EStateQueryResult::Failed;
		}
	}
	return EStateQueryResult::Succeeded;
}