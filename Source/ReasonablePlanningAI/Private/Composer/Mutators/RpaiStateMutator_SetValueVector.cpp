// Radaway Software LLC. 2025. All rights reserved.


#include "Composer/Mutators/RpaiStateMutator_SetValueVector.h"
#include "Core/RpaiState.h"

void URpaiStateMutator_SetValueVector::ReceiveMutate_Implementation(URpaiState* StateToMutate) const
{
	StateToMutate->SetVector(StatePropertyToMutate.StateKeyName, ValueToSet);
}

bool URpaiStateMutator_SetValueVector::CanApply_Implementation(const URpaiState* StateToMutate) const
{
	return StatePropertyToMutate.ExpectedValueType == EStatePropertyType::Vector && StateToMutate->HasVector(StatePropertyToMutate.StateKeyName);
}