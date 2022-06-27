// Troll Purse. All rights reserved.


#include "Composer/Distances/Distance_Vector.h"
#include "Core/ReasonablePlanningState.h"

//Return distance as the difference between the left hand property and the right hand property.
float UDistance_Vector::ReceiveCalculateDistance_Implementation(const UReasonablePlanningState* GivenState) const
{
    if(LHS.ExpectedValueType != EStatePropertyType::Vector)
    {
        UE_LOG(LogRPAI, Warning, TEXT("ExpectedValueType for LHS was not vector."));
        return TNumericLimits<float>::Max();
    }
    FVector fLHS;
    if (GivenState->GetVector(LHS.StateKeyName, fLHS))
    {
        return FVector::DistSquared(fLHS, RHS);
    }
    return TNumericLimits<float>::Max();
}
