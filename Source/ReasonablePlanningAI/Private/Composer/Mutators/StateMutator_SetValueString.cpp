// Troll Purse. All rights reserved.


#include "Composer/Mutators/StateMutator_SetValueString.h"
#include "Core/ReasonablePlanningState.h"

void UStateMutator_SetValueString::ReceiveMutate_Implementation(UReasonablePlanningState* StateToMutate) const
{
	StateToMutate->SetString(StatePropertyToMutate.StateKeyName, ValueToSet);
}

bool UStateMutator_SetValueString::CanApply_Implementation(const UReasonablePlanningState* StateToMutate) const
{
	return StatePropertyToMutate.ExpectedValueType == EStatePropertyType::String && StateToMutate->HasString(StatePropertyToMutate.StateKeyName);
}