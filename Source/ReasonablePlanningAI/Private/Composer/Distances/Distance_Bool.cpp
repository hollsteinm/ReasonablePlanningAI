// Troll Purse. All rights reserved.


#include "Composer/Distances/Distance_Bool.h"
#include "Core/ReasonablePlanningState.h"

//Return distance as the difference between the left hand property and the right hand property.
float UDistance_Bool::ReceiveCalculateDistance_Implementation(const UReasonablePlanningState* GivenState) const
{
    if(LHS.ExpectedValueType != EStatePropertyType::Bool)
    {
        return TNumericLimits<float>::Max();
    }

    bool bLHS = false;
    if (GivenState->GetValueOfType(LHS.StateKeyName, bLHS))
    {
        return (bLHS == bRHS) ? 0.f : 1.f;
    }
    return TNumericLimits<float>::Max();
}
