// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.


#include "Composer/Mutators/RpaiStateMutator_SetValueName.h"
#include "Core/RpaiState.h"

void URpaiStateMutator_SetValueName::ReceiveMutate_Implementation(URpaiState* StateToMutate) const
{
   StateToMutate->SetNameValue(StatePropertyToMutate.StateKeyName, ValueToSet);
}

bool URpaiStateMutator_SetValueName::CanApply_Implementation(const URpaiState* StateToMutate) const
{
   return StatePropertyToMutate.ExpectedValueType == EStatePropertyType::Name && StateToMutate->HasName(StatePropertyToMutate.StateKeyName);
}