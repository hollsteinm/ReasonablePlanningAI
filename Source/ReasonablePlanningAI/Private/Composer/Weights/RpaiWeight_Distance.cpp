// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.


#include "Composer/Weights/RpaiWeight_Distance.h"
#include "Composer/RpaiComposerDistance.h"

float URpaiWeight_Distance::ReceiveExecutionWeight_Implementation(const URpaiState* GivenState) const
{
    check(GivenState != nullptr);
    check(Distance != nullptr);
    return Distance->CalculateDistance(GivenState);
}
