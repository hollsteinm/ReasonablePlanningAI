// Troll Purse. All rights reserved.


#include "Composer/Mutators/StateMutator_SetValueRotator.h"
#include "Core/ReasonablePlanningState.h"

void UStateMutator_SetValueRotator::ReceiveMutate_Implementation(UReasonablePlanningState* StateToMutate) const
{
	StateToMutate->SetRotator(StatePropertyToMutate.StateKeyName, ValueToSet);
}

bool UStateMutator_SetValueRotator::CanApply_Implementation(const UReasonablePlanningState* StateToMutate) const
{
	return StatePropertyToMutate.ExpectedValueType == EStatePropertyType::Rotator && StateToMutate->HasRotator(StatePropertyToMutate.StateKeyName);
}