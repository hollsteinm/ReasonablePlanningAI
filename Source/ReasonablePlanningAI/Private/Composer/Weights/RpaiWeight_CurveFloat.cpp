// Radaway Software LLC. 2025. All rights reserved.


#include "Composer/Weights/RpaiWeight_CurveFloat.h"
#include "Core/RpaiState.h"

float URpaiWeight_CurveFloat::ReceiveExecutionWeight_Implementation(const URpaiState* GivenState) const
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
