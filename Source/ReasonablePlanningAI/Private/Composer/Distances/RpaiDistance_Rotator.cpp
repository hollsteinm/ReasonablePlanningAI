// Radaway Software LLC. 2025. All rights reserved.


#include "Composer/Distances/RpaiDistance_Rotator.h"
#include "Core/RpaiState.h"

//Return distance as the difference between the left hand property and the right hand property.
float URpaiDistance_Rotator::ReceiveCalculateDistance_Implementation(const URpaiState* GivenState) const
{
    if(LHS.ExpectedValueType != EStatePropertyType::Rotator)
    {
        return TNumericLimits<float>::Max();
    }
    FRotator rLHS;
    if (GivenState->GetValueOfType(LHS.StateKeyName, rLHS))
    {
        return rLHS.GetManhattanDistance(RHS);
    }
    return TNumericLimits<float>::Max();
}
