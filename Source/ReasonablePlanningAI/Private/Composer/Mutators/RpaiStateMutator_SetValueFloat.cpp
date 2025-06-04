// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.


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