// Troll Purse. All rights reserved.


#include "Composer/Queries/RpaiStateQuery_CompareDistanceFloat.h"
#include "Composer/RpaiComposerDistance.h"

EStateQueryResult URpaiStateQuery_CompareDistanceFloat::Query(const URpaiState* StateToQuery) const
{
    check(StateToQuery != nullptr)
    check(Distance != nullptr)
    float LHS = Distance->CalculateDistance(StateToQuery);
    return DoCompare(ComparisonOperation, LHS, RHS);
}
