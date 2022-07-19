// Troll Purse. All rights reserved.


#include "Composer/Distances/Distance_AddAll.h"

float UDistance_AddAll::ReceiveCalculateDistance_Implementation(const UReasonablePlanningState* GivenState) const
{
	float Result = 0.f;
	for (const auto Distance : SubDistances)
	{
		Result += Distance->CalculateDistance(GivenState);
	}
	return Result;
}