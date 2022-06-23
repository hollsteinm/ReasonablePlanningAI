// Troll Purse. All rights reserved.


#include "Composer/Mutators/StateMutator_SetValueVector.h"
#include "Core/ReasonablePlanningState.h"

void UStateMutator_SetValueVector::ReceiveMutate_Implementation(UReasonablePlanningState* StateToMutate) const
{
	StateToMutate->SetVector(StatePropertyToMutate.StateKeyName, ValueToSet);
}

bool UStateMutator_SetValueVector::CanApply_Implementation(const UReasonablePlanningState* StateToMutate) const
{
	return StatePropertyToMutate.ExpectedValueType == EStatePropertyType::Vector && StateToMutate->HasVector(StatePropertyToMutate.StateKeyName);
}