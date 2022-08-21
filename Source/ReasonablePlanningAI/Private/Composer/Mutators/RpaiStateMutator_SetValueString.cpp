// Troll Purse. All rights reserved.


#include "Composer/Mutators/RpaiStateMutator_SetValueString.h"
#include "Core/RpaiState.h"

void URpaiStateMutator_SetValueString::ReceiveMutate_Implementation(URpaiState* StateToMutate) const
{
	StateToMutate->SetString(StatePropertyToMutate.StateKeyName, ValueToSet);
}

bool URpaiStateMutator_SetValueString::CanApply_Implementation(const URpaiState* StateToMutate) const
{
	return StatePropertyToMutate.ExpectedValueType == EStatePropertyType::String && StateToMutate->HasString(StatePropertyToMutate.StateKeyName);
}