// Radaway Software LLC. 2025. All rights reserved.


#include "Composer/Distances/RpaiDistance_Integer.h"
#include "Core/RpaiState.h"

//Return distance as the difference between the left hand property and the right hand property.
float URpaiDistance_Integer::ReceiveCalculateDistance_Implementation(const URpaiState* GivenState) const
{
    if(LHS.ExpectedValueType != EStatePropertyType::Int && LHS.ExpectedValueType != EStatePropertyType::Float)
    {
        return TNumericLimits<float>::Max();
    }
    int32 iLHS;
    float fLHS;
    if (GivenState->GetValueOfType(LHS.StateKeyName, iLHS))
    {
        return float(iLHS - RHS);
    }
    else if(GivenState->GetValueOfType(LHS.StateKeyName, fLHS))
    {
        return fLHS - float(RHS);
    }
    else
    {
        return TNumericLimits<float>::Max();
    }
}
