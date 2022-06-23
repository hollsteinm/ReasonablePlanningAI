// Troll Purse. All rights reserved.


#include "Composer/Mutators/StateMutator_SetValueName.h"
#include "Core/ReasonablePlanningState.h"

void UStateMutator_SetValueName::ReceiveMutate_Implementation(UReasonablePlanningState* StateToMutate) const
{
	StateToMutate->SetNameValue(StatePropertyToMutate.StateKeyName, ValueToSet);
}

bool UStateMutator_SetValueName::CanApply_Implementation(const UReasonablePlanningState* StateToMutate) const
{
	return StatePropertyToMutate.ExpectedValueType == EStatePropertyType::Name && StateToMutate->HasName(StatePropertyToMutate.StateKeyName);
}