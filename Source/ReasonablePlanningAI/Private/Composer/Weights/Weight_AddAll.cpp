// Troll Purse. All rights reserved.


#include "Composer/Weights/Weight_AddAll.h"

float UWeight_AddAll::ReceiveExecutionWeight_Implementation(const UReasonablePlanningState* GivenState) const
{
	float Result = 0.f;
	for (const auto Weight : SubWeights)
	{
		Result += Weight->ExecutionWeight(GivenState);
	}
	return Result;
}