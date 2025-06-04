// Radaway Software LLC. 2025. All rights reserved.


#include "Composer/Mutators/RpaiStateMutator_SetValueInteger.h"
#include "Core/RpaiState.h"

void URpaiStateMutator_SetValueInteger::ReceiveMutate_Implementation(URpaiState* StateToMutate) const
{
   StateToMutate->SetInt(StatePropertyToMutate.StateKeyName, ValueToSet);
}

bool URpaiStateMutator_SetValueInteger::CanApply_Implementation(const URpaiState* StateToMutate) const
{
   return StatePropertyToMutate.ExpectedValueType == EStatePropertyType::Int && StateToMutate->HasInt(StatePropertyToMutate.StateKeyName);
}