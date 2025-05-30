// Radaway Software LLC. 2025. All rights reserved.


#include "Composer/Distances/RpaiDistance_AddAll.h"

float URpaiDistance_AddAll::ReceiveCalculateDistance_Implementation(const URpaiState* GivenState) const
{
	float Result = 0.f;
	for (const auto Distance : SubDistances)
	{
		Result += Distance->CalculateDistance(GivenState);
	}
	return Result;
}