// Troll Purse. All rights reserved.


#include "Composer/Distances/Distance_Rotator.h"
#include "Core/ReasonablePlanningState.h"

//Return distance as the difference between the left hand property and the right hand property.
float UDistance_Rotator::ReceiveCalculateDistance_Implementation(const UReasonablePlanningState* GivenState) const
{
    if(LHS.ExpectedValueType != EStatePropertyType::Rotator)
    {
        UE_LOG(LogRPAI, Warning, TEXT("ExpectedValueType for LHS was not rotator."));
        return TNumericLimits<float>::Max();
    }
    FRotator rLHS;
    if (GivenState->GetRotator(LHS.StateKeyName, rLHS))
    {
        return rLHS.GetManhattanDistance(RHS);
    }
    return TNumericLimits<float>::Max();
}
