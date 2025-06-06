// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.


#include "Composer/Mutators/RpaiStateMutator_SetValueRotator.h"
#include "Core/RpaiState.h"

void URpaiStateMutator_SetValueRotator::ReceiveMutate_Implementation(URpaiState* StateToMutate) const
{
   StateToMutate->SetRotator(StatePropertyToMutate.StateKeyName, ValueToSet);
}

bool URpaiStateMutator_SetValueRotator::CanApply_Implementation(const URpaiState* StateToMutate) const
{
   return StatePropertyToMutate.ExpectedValueType == EStatePropertyType::Rotator && StateToMutate->HasRotator(StatePropertyToMutate.StateKeyName);
}