// Radaway Software LLC. 2025. All rights reserved.


#include "Composer/Distances/RpaiDistance_Float.h"
#include "Core/RpaiState.h"

//Return distance as the difference between the left hand property and the right hand property.
float URpaiDistance_Float::ReceiveCalculateDistance_Implementation(const URpaiState* GivenState) const
{
    if(LHS.ExpectedValueType != EStatePropertyType::Float && LHS.ExpectedValueType != EStatePropertyType::Int)
    {
        return TNumericLimits<float>::Max();
    }

    float fLHS;
    int32 iLHS;
    if (GivenState->GetValueOfType(LHS.StateKeyName, fLHS))
    {
        return fLHS - RHS;
    }
    else if (GivenState->GetValueOfType(LHS.StateKeyName, iLHS))
    {
        return iLHS - FMath::FloorToInt(RHS);
    }
    else
    {
        return TNumericLimits<float>::Max();
    }
}
