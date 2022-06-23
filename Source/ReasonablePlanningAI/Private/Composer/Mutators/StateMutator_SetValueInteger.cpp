// Troll Purse. All rights reserved.


#include "Composer/Mutators/StateMutator_SetValueInteger.h"
#include "Core/ReasonablePlanningState.h"

void UStateMutator_SetValueInteger::ReceiveMutate_Implementation(UReasonablePlanningState* StateToMutate) const
{
	StateToMutate->SetInt(StatePropertyToMutate.StateKeyName, ValueToSet);
}

bool UStateMutator_SetValueInteger::CanApply_Implementation(const UReasonablePlanningState* StateToMutate) const
{
	return StatePropertyToMutate.ExpectedValueType == EStatePropertyType::Int && StateToMutate->HasInt(StatePropertyToMutate.StateKeyName);
}