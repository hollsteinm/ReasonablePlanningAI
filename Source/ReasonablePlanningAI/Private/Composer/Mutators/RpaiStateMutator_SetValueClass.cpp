// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.


#include "Composer/Mutators/RpaiStateMutator_SetValueClass.h"
#include "Core/RpaiState.h"

void URpaiStateMutator_SetValueClass::ReceiveMutate_Implementation(URpaiState* StateToMutate) const
{
   StateToMutate->SetClassValue(StatePropertyToMutate.StateKeyName, ValueToSet);
}

bool URpaiStateMutator_SetValueClass::CanApply_Implementation(const URpaiState* StateToMutate) const
{
   return StatePropertyToMutate.ExpectedValueType == EStatePropertyType::Class && StateToMutate->HasClass(StatePropertyToMutate.StateKeyName);
}