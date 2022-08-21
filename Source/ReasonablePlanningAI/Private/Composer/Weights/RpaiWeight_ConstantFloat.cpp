// Troll Purse. All rights reserved.


#include "Composer/Weights/RpaiWeight_ConstantFloat.h"

URpaiWeight_ConstantFloat::URpaiWeight_ConstantFloat()
	: ConstantWeight(0.f)
{

}

float URpaiWeight_ConstantFloat::ReceiveExecutionWeight_Implementation(const URpaiState* GivenState) const
{
	return ConstantWeight;
}
