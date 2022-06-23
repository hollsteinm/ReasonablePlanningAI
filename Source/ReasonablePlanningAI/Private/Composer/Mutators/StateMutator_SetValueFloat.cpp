// Troll Purse. All rights reserved.


#include "Composer/Mutators/StateMutator_SetValueFloat.h"
#include "Core/ReasonablePlanningState.h"

void UStateMutator_SetValueFloat::ReceiveMutate_Implementation(UReasonablePlanningState* StateToMutate) const
{
	StateToMutate->SetFloat(StatePropertyToMutate.StateKeyName, ValueToSet);
}

bool UStateMutator_SetValueFloat::CanApply_Implementation(const UReasonablePlanningState* StateToMutate) const
{
	return StatePropertyToMutate.ExpectedValueType == EStatePropertyType::Float && StateToMutate->HasFloat(StatePropertyToMutate.StateKeyName);
}