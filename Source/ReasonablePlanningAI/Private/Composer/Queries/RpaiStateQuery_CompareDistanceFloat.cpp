// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.


#include "Composer/Queries/RpaiStateQuery_CompareDistanceFloat.h"
#include "Composer/RpaiComposerDistance.h"

EStateQueryResult URpaiStateQuery_CompareDistanceFloat::ReceiveQuery_Implementation(const URpaiState* StateToQuery) const
{
    check(StateToQuery != nullptr)
    check(Distance != nullptr)
    float LHS = Distance->CalculateDistance(StateToQuery);
    return DoCompare(ComparisonOperation, LHS, RHS);
}
