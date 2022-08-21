// Troll Purse. All rights reserved.


#include "Composer/Queries/RpaiStateQuery_CompareToDistance.h"
#include "Composer/RpaiComposerDistance.h"
#include "Core/RpaiState.h"

EStateQueryResult URpaiStateQuery_CompareToDistance::Query(const URpaiState* StateToQuery) const
{
    if (QueriedState.ExpectedValueType != EStatePropertyType::Float)
    {
        return EStateQueryResult::Invalid;
    }

    float Value;
    if (!StateToQuery->GetFloat(QueriedState.StateKeyName, Value))
    {
        return EStateQueryResult::Invalid;
    }
    
    float ValueToCompare = Distance->CalculateDistance(StateToQuery);
    return DoCompare(ComparisonOperation, Value, ValueToCompare);
}

