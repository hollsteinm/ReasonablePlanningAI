// Radaway Software LLC. 2025. All rights reserved.


#include "Composer/Mutators/RpaiStateMutator_SetValueBool.h"
#include "Core/RpaiState.h"

void URpaiStateMutator_SetValueBool::ReceiveMutate_Implementation(URpaiState* StateToMutate) const
{
	StateToMutate->SetBool(StatePropertyToMutate.StateKeyName, bValueToSet);
}

bool URpaiStateMutator_SetValueBool::CanApply_Implementation(const URpaiState* StateToMutate) const
{
	return StatePropertyToMutate.ExpectedValueType == EStatePropertyType::Bool && StateToMutate->HasBool(StatePropertyToMutate.StateKeyName);
}