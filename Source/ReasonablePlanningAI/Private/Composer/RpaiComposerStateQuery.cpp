// Troll Purse. All rights reserved.


#include "Composer/RpaiComposerStateQuery.h"
#include "Core/RpaiState.h"

DECLARE_CYCLE_STAT(TEXT("Query State"), STAT_QueryState, STATGROUP_Rpai);

EStateQueryResult URpaiComposerStateQuery::Query(const URpaiState* StateToQuery) const
{
	check(StateToQuery != nullptr)
	SCOPE_CYCLE_COUNTER(STAT_QueryState);
	return ReceiveQuery(StateToQuery);
}

EStateQueryResult URpaiComposerStateQuery::ReceiveQuery_Implementation(const URpaiState* StateToQuery) const
{
	return EStateQueryResult::Invalid;
}