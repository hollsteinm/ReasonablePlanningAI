// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.

#include "Composer/RpaiComposerDistanceBase.h"
#include "Core/RpaiState.h"

DECLARE_CYCLE_STAT(TEXT("Calculate Distance"), STAT_CalculateDistance, STATGROUP_Rpai);

float URpaiComposerDistanceBase::CalculateDistance(const URpaiState* GivenState) const
{
    check(GivenState != nullptr)
    SCOPE_CYCLE_COUNTER(STAT_CalculateDistance);
    return FMath::Abs(ReceiveCalculateDistance(GivenState));
}

//Return distance as the difference between the left hand property and the right hand property.
float URpaiComposerDistanceBase::ReceiveCalculateDistance_Implementation(const URpaiState* GivenState) const
{
    return 0.f;
}
