// Troll Purse. All rights reserved.


#include "Composer/ReasonablePlanningWeightBase.h"

float UReasonablePlanningWeightBase::ExecutionWeight(const UReasonablePlanningState* GivenState) const
{
	return ReceiveExecutionWeight(GivenState);
}

float UReasonablePlanningWeightBase::ReceiveExecutionWeight_Implementation(const UReasonablePlanningState* GivenState) const
{
	return 0.f;
}
