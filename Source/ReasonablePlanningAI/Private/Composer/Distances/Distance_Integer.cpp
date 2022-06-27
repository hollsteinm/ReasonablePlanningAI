// Troll Purse. All rights reserved.


#include "Composer/Distances/Distance_Integer.h"
#include "Core/ReasonablePlanningState.h"

//Return distance as the difference between the left hand property and the right hand property.
float UDistance_Integer::ReceiveCalculateDistance_Implementation(const UReasonablePlanningState* GivenState) const
{
    if(LHS.ExpectedValueType != EStatePropertyType::Int)
    {
        UE_LOG(LogRPAI, Warning, TEXT("ExpectedValueType for LHS was not Bool."));
        return TNumericLimits<float>::Max();
    }
    int32 iLHS;
    if (GivenState->GetInt(LHS.StateKeyName, iLHS))
    {
        return float(iLHS - RHS);
    }
    return TNumericLimits<float>::Max();
}
