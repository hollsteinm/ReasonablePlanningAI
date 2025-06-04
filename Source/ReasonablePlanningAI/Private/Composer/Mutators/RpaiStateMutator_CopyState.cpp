// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.


#include "Composer/Mutators/RpaiStateMutator_CopyState.h"
#include "Core/RpaiState.h"

void URpaiStateMutator_CopyState::ReceiveMutate_Implementation(URpaiState* StateToChange) const
{
   switch (StatePropertyToCopy.ExpectedValueType)
   {
   case EStatePropertyType::Bool:
   {
      bool bValue = false;
      if (StateToChange->GetValueOfType(StatePropertyToCopy.StateKeyName, bValue))
      {
         StateToChange->SetValueOfType(StatePropertyToMutate.StateKeyName, bValue);
      }
   }
   break;
   case EStatePropertyType::Class:
   {
      UClass* Value = nullptr;
      if (StateToChange->GetValueOfType(StatePropertyToCopy.StateKeyName, Value))
      {
         StateToChange->SetValueOfType(StatePropertyToMutate.StateKeyName, Value);
      }
   }
   break;
   case EStatePropertyType::Float:
   {
      float Value;
      if (StateToChange->GetValueOfType(StatePropertyToCopy.StateKeyName, Value))
      {
         StateToChange->SetValueOfType(StatePropertyToMutate.StateKeyName, Value);
      }
   }
   break;
   case EStatePropertyType::Int:
   {
      int32 Value;
      if (StateToChange->GetValueOfType(StatePropertyToCopy.StateKeyName, Value))
      {
         StateToChange->SetValueOfType(StatePropertyToMutate.StateKeyName, Value);
      }
   }
   break;
   case EStatePropertyType::Name:
   {
      FName Value;
      if (StateToChange->GetValueOfType(StatePropertyToCopy.StateKeyName, Value))
      {
         StateToChange->SetValueOfType(StatePropertyToMutate.StateKeyName, Value);
      }
   }
   break;
   case EStatePropertyType::Object:
   {
      UObject* Value;
      if (StateToChange->GetValueOfType(StatePropertyToCopy.StateKeyName, Value))
      {
         StateToChange->SetValueOfType(StatePropertyToMutate.StateKeyName, Value);
      }
   }
   break;
   case EStatePropertyType::Rotator:
   {
      FRotator Value;
      if (StateToChange->GetValueOfType(StatePropertyToCopy.StateKeyName, Value))
      {
         StateToChange->SetValueOfType(StatePropertyToMutate.StateKeyName, Value);
      }
   }
   break;
   case EStatePropertyType::String:
   {
      FString Value;
      if (StateToChange->GetValueOfType(StatePropertyToCopy.StateKeyName, Value))
      {
         StateToChange->SetValueOfType(StatePropertyToMutate.StateKeyName, Value);
      }
   }
   break;
   case EStatePropertyType::Vector:
   {
      FVector Value;
      if (StateToChange->GetValueOfType(StatePropertyToCopy.StateKeyName, Value))
      {
         StateToChange->SetValueOfType(StatePropertyToMutate.StateKeyName, Value);
      }
   }
   break;
   default:
      break;
   }
}

bool URpaiStateMutator_CopyState::CanApply_Implementation(const URpaiState* StateToChange) const
{
   return StateToChange->HasReferencedState(StatePropertyToCopy) && StatePropertyToCopy.ExpectedValueType == StatePropertyToMutate.ExpectedValueType;
}