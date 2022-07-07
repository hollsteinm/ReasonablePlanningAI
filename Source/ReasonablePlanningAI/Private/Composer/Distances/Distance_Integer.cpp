// Troll Purse. All rights reserved.


#include "Composer/Distances/Distance_Integer.h"
#include "Core/ReasonablePlanningState.h"

//Return distance as the difference between the left hand property and the right hand property.
float UDistance_Integer::ReceiveCalculateDistance_Implementation(const UReasonablePlanningState* GivenState) const
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
