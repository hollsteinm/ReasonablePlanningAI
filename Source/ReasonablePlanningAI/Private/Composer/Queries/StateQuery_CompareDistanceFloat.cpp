// Troll Purse. All rights reserved.


#include "Composer/Queries/StateQuery_CompareDistanceFloat.h"
#include "Composer/ReasonablePlanningDistance.h"

EStateQueryResult UStateQuery_CompareDistanceFloat::Query(const UReasonablePlanningState* StateToQuery) const
{
    check(StateToQuery != nullptr)
    check(Distance != nullptr)
    float LHS = Distance->CalculateDistance(StateToQuery);
    return DoCompare(ComparisonOperation, LHS, RHS);
}
