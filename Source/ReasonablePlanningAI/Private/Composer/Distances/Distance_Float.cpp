// Troll Purse. All rights reserved.


#include "Composer/Distances/Distance_Float.h"
#include "Core/ReasonablePlanningState.h"

//Return distance as the difference between the left hand property and the right hand property.
float UDistance_Float::ReceiveCalculateDistance_Implementation(const UReasonablePlanningState* GivenState) const
{
    if(LHS.ExpectedValueType != EStatePropertyType::Float)
    {
        UE_LOG(LogRPAI, Warning, TEXT("ExpectedValueType for LHS was not Float."));
        return TNumericLimits<float>::Max();
    }
    float fLHS;
    if (GivenState->GetFloat(LHS.StateKeyName, fLHS))
    {
        return fLHS - RHS;
    }
    return TNumericLimits<float>::Max();
}
