// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.


#include "Composer/RpaiComposerWeightBase.h"

float URpaiComposerWeightBase::ExecutionWeight(const URpaiState* GivenState) const
{
   return ReceiveExecutionWeight(GivenState);
}

float URpaiComposerWeightBase::ReceiveExecutionWeight_Implementation(const URpaiState* GivenState) const
{
   return 0.f;
}
