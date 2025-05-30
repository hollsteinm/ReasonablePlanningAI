// Radaway Software LLC. 2025. All rights reserved.


#include "Composer/Distances/RpaiDistance_Vector.h"
#include "Core/RpaiState.h"

//Return distance as the difference between the left hand property and the right hand property.
float URpaiDistance_Vector::ReceiveCalculateDistance_Implementation(const URpaiState* GivenState) const
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
