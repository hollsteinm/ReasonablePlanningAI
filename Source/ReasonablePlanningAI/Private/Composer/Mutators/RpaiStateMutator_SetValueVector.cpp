// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.


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