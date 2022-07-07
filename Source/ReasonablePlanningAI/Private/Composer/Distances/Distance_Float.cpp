// Troll Purse. All rights reserved.


#include "Composer/Distances/Distance_Float.h"
#include "Core/ReasonablePlanningState.h"

//Return distance as the difference between the left hand property and the right hand property.
float UDistance_Float::ReceiveCalculateDistance_Implementation(const UReasonablePlanningState* GivenState) const
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
