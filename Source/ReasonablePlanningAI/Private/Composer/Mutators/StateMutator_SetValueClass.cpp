// Troll Purse. All rights reserved.


#include "Composer/Mutators/StateMutator_SetValueClass.h"
#include "Core/ReasonablePlanningState.h"

void UStateMutator_SetValueClass::ReceiveMutate_Implementation(UReasonablePlanningState* StateToMutate) const
{
	StateToMutate->SetClassValue(StatePropertyToMutate.StateKeyName, ValueToSet);
}

bool UStateMutator_SetValueClass::CanApply_Implementation(const UReasonablePlanningState* StateToMutate) const
{
	return StatePropertyToMutate.ExpectedValueType == EStatePropertyType::Class && StateToMutate->HasClass(StatePropertyToMutate.StateKeyName);
}