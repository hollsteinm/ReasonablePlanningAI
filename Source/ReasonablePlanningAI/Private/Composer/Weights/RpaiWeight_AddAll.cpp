// Radaway Software LLC. 2025. All rights reserved.


#include "Composer/Weights/RpaiWeight_AddAll.h"

float URpaiWeight_AddAll::ReceiveExecutionWeight_Implementation(const URpaiState* GivenState) const
{
	float Result = 0.f;
	for (const auto Weight : SubWeights)
	{
		Result += Weight->ExecutionWeight(GivenState);
	}
	return Result;
}