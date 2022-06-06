// Troll Purse. All rights reserved.


#include "Weight_ConstantFloat.h"

UWeight_ConstantFloat::UWeight_ConstantFloat()
	: ConstantWeight(0.f)
{

}

float UWeight_ConstantFloat::ReceiveExecutionWeight_Implementation(const UReasonablePlanningState* GivenState) const
{
	return ConstantWeight;
}