// Radaway Software LLC. 2025. All rights reserved.


#include "Composer/Mutators/RpaiStateMutator_SetValueFloat.h"
#include "Core/RpaiState.h"

void URpaiStateMutator_SetValueFloat::ReceiveMutate_Implementation(URpaiState* StateToMutate) const
{
	StateToMutate->SetFloat(StatePropertyToMutate.StateKeyName, ValueToSet);
}

bool URpaiStateMutator_SetValueFloat::CanApply_Implementation(const URpaiState* StateToMutate) const
{
	return StatePropertyToMutate.ExpectedValueType == EStatePropertyType::Float && StateToMutate->HasFloat(StatePropertyToMutate.StateKeyName);
}