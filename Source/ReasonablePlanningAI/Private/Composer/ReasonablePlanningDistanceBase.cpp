// Troll Purse. All rights reserved.

#include "Composer/ReasonablePlanningDistanceBase.h"
#include "Core/ReasonablePlanningState.h"

DECLARE_CYCLE_STAT(TEXT("Calculate Distance"), STAT_CalculateDistance, STATGROUP_RPAI);

float UReasonablePlanningDistanceBase::CalculateDistance(const UReasonablePlanningState* GivenState) const
{
    check(GivenState != nullptr)
    SCOPE_CYCLE_COUNTER(STAT_CalculateDistance);
    return FMath::Abs(ReceiveCalculateDistance(GivenState));
}

//Return distance as the difference between the left hand property and the right hand property.
float UReasonablePlanningDistanceBase::ReceiveCalculateDistance_Implementation(const UReasonablePlanningState* GivenState) const
{
    return TNumericLimits<float>::Max();
}
