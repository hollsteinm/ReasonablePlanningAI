// Troll Purse. All rights reserved.


#include "Weight_CurveFloat.h"
#include "ReasonablePlanningState.h"

float UWeight_CurveFloat::ReceiveExecutionWeight_Implementation(const UReasonablePlanningState* GivenState) const
{
	float ReturnValue = 0.f;
	if (GivenState->GetFloat(WeightStateKeyValueReference.StateKeyName, ReturnValue))
	{
		return WeightFloatCurve->GetFloatValue(ReturnValue);
	}
	return ReturnValue;
}
