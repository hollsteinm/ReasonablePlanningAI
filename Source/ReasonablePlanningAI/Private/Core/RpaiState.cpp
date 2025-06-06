// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.


#include "Core/RpaiState.h"

bool URpaiState::IsEqualTo_Implementation(const URpaiState* OtherState) const
{
   return false;
}

bool URpaiState::HasReferencedState(const FStateKeyValueReference& StatePropertyReference) const
{
   return !StatePropertyReference.StateKeyName.IsNone() &&
      HasValueWithName(StatePropertyReference.StateKeyName) &&
      StatePropertyReference.ExpectedValueType != EStatePropertyType::Invalid &&
      StatePropertyReference.ExpectedValueType == GetTypeFromName(StatePropertyReference.StateKeyName);
}

void URpaiState::SetStateFromController_Implementation(const AAIController* FromController)
{
   
}
