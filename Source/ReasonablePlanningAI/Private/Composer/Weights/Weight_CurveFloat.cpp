// Troll Purse. All rights reserved.


#include "Composer/Weights/Weight_CurveFloat.h"
#include "Core/ReasonablePlanningState.h"

float UWeight_CurveFloat::ReceiveExecutionWeight_Implementation(const UReasonablePlanningState* GivenState) const
{
	float FloatInput = 0.f;
	int32 IntegerInput = 0;
	if (GivenState->GetFloat(WeightStateKeyValueReference.StateKeyName, FloatInput))
	{
		return WeightFloatCurve->GetFloatValue(FloatInput);
	}
	else if (GivenState->GetInt(WeightStateKeyValueReference.StateKeyName, IntegerInput))
	{
		return WeightFloatCurve->GetFloatValue(static_cast<float>(IntegerInput));
	}
	return 0.f;
}
