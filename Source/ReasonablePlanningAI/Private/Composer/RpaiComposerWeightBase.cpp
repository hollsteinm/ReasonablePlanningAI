// Radaway Software LLC. 2025. All rights reserved.


#include "Composer/RpaiComposerWeightBase.h"

float URpaiComposerWeightBase::ExecutionWeight(const URpaiState* GivenState) const
{
	return ReceiveExecutionWeight(GivenState);
}

float URpaiComposerWeightBase::ReceiveExecutionWeight_Implementation(const URpaiState* GivenState) const
{
	return 0.f;
}
