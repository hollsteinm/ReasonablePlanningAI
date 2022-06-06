// Troll Purse. All rights reserved.


#include "Weight_Float.h"
#include "ReasonablePlanningState.h"

float UWeight_Float::ReceiveExecutionWeight_Implementation(const UReasonablePlanningState* GivenState) const
{
	float ReturnValue = 0.f;
	if (!GivenState->GetFloat(WeightStateKeyValueReference.StateKeyName, ReturnValue))
	{
		return 0.f;
	}
	return ReturnValue;
}