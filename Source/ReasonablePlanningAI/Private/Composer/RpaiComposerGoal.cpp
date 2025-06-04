// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.


#include "Composer/RpaiComposerGoal.h"
#include "Composer/RpaiComposerStateQuery.h"
#include "Composer/RpaiComposerWeightBase.h"
#include "Composer/RpaiComposerDistance.h"

bool URpaiComposerGoal::ReceiveIsInDesiredState_Implementation(const URpaiState* GivenState) const
{
    check(GivenState != nullptr);
    check(IsInDesiredStateQuery != nullptr);
    return IsInDesiredStateQuery->Query(GivenState) == EStateQueryResult::Succeeded;
}

float URpaiComposerGoal::ReceiveGetDistanceToDesiredState_Implementation(const URpaiState* GivenState) const
{
    check(GivenState != nullptr);
    check(DistanceCalculator != nullptr);
    return DistanceCalculator->CalculateDistance(GivenState);
}

bool URpaiComposerGoal::ReceiveIsApplicable_Implementation(const URpaiState* GivenState) const
{
    check(GivenState != nullptr);
    check(IsApplicableQuery != nullptr);
    return IsApplicableQuery->Query(GivenState) == EStateQueryResult::Succeeded;
}

float URpaiComposerGoal::ReceiveGetWeight_Implementation(const URpaiState* GivenState) const
{
    check(GivenState != nullptr);
    check(Weight != nullptr);
    return Weight->ExecutionWeight(GivenState);
}
