// Troll Purse. All rights reserved.

#include "Composer/ReasonablePlanningDistance.h"
#include "Core/ReasonablePlanningState.h"

DECLARE_CYCLE_STAT(TEXT("Calculate Distance"), STAT_CalculateDistance, STATGROUP_RPAI);

float UReasonablePlanningDistance::CalculateDistance(const UReasonablePlanningState* GivenState) const
{
	SCOPE_CYCLE_COUNTER(STAT_CalculateDistance);
    if (GivenState == nullptr)
    {
        UE_LOG(LogRPAI, Warning, TEXT("GivenState was null, returning max float"));
        return TNumericLimits<float>::Max();
    }
    if (!GivenState->HasReferencedState(LHS))
    {
        UE_LOG(LogRPAI, Warning, TEXT("Unable to calculate distance from %s, state does not have referenced keys. Types for LHS %s. For state %s"),
            *LHS.StateKeyName.ToString(),
            *UEnum::GetValueAsName(LHS.ExpectedValueType).ToString(),
            *GivenState->GetName()
        );
        return TNumericLimits<float>::Max();
    }
	return FMath::Abs(ReceiveCalculateDistance(GivenState));
}

//Return distance as the difference between the left hand property and the right hand property.
float UReasonablePlanningDistance::ReceiveCalculateDistance_Implementation(const UReasonablePlanningState* GivenState) const
{
    return TNumericLimits<float>::Max();
}
