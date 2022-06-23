// Troll Purse. All rights reserved.


#include "Composer/Mutators/StateMutator_SetValueBool.h"
#include "Core/ReasonablePlanningState.h"

void UStateMutator_SetValueBool::ReceiveMutate_Implementation(UReasonablePlanningState* StateToMutate) const
{
	StateToMutate->SetBool(StatePropertyToMutate.StateKeyName, bValueToSet);
}

bool UStateMutator_SetValueBool::CanApply_Implementation(const UReasonablePlanningState* StateToMutate) const
{
	return StatePropertyToMutate.ExpectedValueType == EStatePropertyType::Bool && StateToMutate->HasBool(StatePropertyToMutate.StateKeyName);
}