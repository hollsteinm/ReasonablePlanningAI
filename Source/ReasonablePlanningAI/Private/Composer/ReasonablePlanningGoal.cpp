// Troll Purse. All rights reserved.


#include "Composer/ReasonablePlanningGoal.h"
#include "Composer/ReasonablePlanningStateQuery.h"
#include "Composer/ReasonablePlanningWeightBase.h"
#include "Composer/ReasonablePlanningDistance.h"

bool UReasonablePlanningGoal::ReceiveIsInDesiredState_Implementation(const UReasonablePlanningState* GivenState) const
{
    check(GivenState != nullptr);
    check(IsInDesiredStateQuery != nullptr);
    return IsInDesiredStateQuery->Query(GivenState) == EStateQueryResult::Succeeded;
}

float UReasonablePlanningGoal::ReceiveGetDistanceToDesiredState_Implementation(const UReasonablePlanningState* GivenState) const
{
    check(GivenState != nullptr);
    check(DistanceCalculator != nullptr);
    return DistanceCalculator->CalculateDistance(GivenState);
}

bool UReasonablePlanningGoal::ReceiveIsApplicable_Implementation(const UReasonablePlanningState* GivenState) const
{
    check(GivenState != nullptr);
    check(IsApplicableQuery != nullptr);
    return IsApplicableQuery->Query(GivenState) == EStateQueryResult::Succeeded;
}

float UReasonablePlanningGoal::ReceiveGetWeight_Implementation(const UReasonablePlanningState* GivenState) const
{
    check(GivenState != nullptr);
    check(Weight != nullptr);
    return Weight->ExecutionWeight(GivenState);
}
