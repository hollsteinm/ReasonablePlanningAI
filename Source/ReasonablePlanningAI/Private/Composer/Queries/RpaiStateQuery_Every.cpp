// Troll Purse. All rights reserved.


#include "Composer/Queries/RpaiStateQuery_Every.h"

EStateQueryResult URpaiStateQuery_Every::Query(const URpaiState* StateToQuery) const
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