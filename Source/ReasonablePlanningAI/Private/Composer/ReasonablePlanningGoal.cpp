// Troll Purse. All rights reserved.


#include "Composer/ReasonablePlanningGoal.h"
#include "Composer/ReasonablePlanningStateQuery.h"
#include "Composer/ReasonablePlanningWeightBase.h"
#include "Composer/ReasonablePlanningDistance.h"

bool UReasonablePlanningGoal::ReceiveIsInDesiredState_Implementation(const UReasonablePlanningState* GivenState) const
{
    check(GivenState != nullptr);
    bool bSuccess = true;
    auto Current = 0;
    const auto End = IsInDesiredStateQueries.Num();
    while(bSuccess && Current != End)
    {
        bSuccess &= IsInDesiredStateQueries[Current++]->Query(GivenState) == EStateQueryResult::Succeeded;
    }
    return bSuccess;
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
    bool bSuccess = true;
    auto Current = 0;
    const auto End = IsApplicableQueries.Num();
    while(bSuccess && Current != End)
    {
        bSuccess &= IsApplicableQueries[Current++]->Query(GivenState) == EStateQueryResult::Succeeded;
    }
    return bSuccess;
}

float UReasonablePlanningGoal::ReceiveGetWeight_Implementation(const UReasonablePlanningState* GivenState) const
{
    check(GivenState != nullptr);
    check(Weight != nullptr);
    return Weight->ExecutionWeight(GivenState);
}
