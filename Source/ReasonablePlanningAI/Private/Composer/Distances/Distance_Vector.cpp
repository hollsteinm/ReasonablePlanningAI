// Troll Purse. All rights reserved.


#include "Composer/Distances/Distance_Vector.h"
#include "Core/ReasonablePlanningState.h"

//Return distance as the difference between the left hand property and the right hand property.
float UDistance_Vector::ReceiveCalculateDistance_Implementation(const UReasonablePlanningState* GivenState) const
{
    if(LHS.ExpectedValueType != EStatePropertyType::Vector)
    {
        return TNumericLimits<float>::Max();
    }
    FVector fLHS;
    if (GivenState->GetValueOfType(LHS.StateKeyName, fLHS))
    {
        return FVector::DistSquared(fLHS, RHS);
    }
    return TNumericLimits<float>::Max();
}
