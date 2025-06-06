// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.

#include "States/RpaiState_BlackboardComponentAdapter.h"
#include "BehaviorTree/BlackboardComponent.h"

template<typename T>
inline void CopyValue(const FName& Key, URpaiState_BlackboardComponentAdapter* State, const T& ValueToCopy)
{
   State->SetValueOfType<T>(Key, ValueToCopy);
}

uint32 URpaiState_BlackboardComponentAdapter::ReadFromBlackboard(UBlackboardComponent* SourceStateBlackboard)
{
   uint32 TotalRead = 0;
   SetAsDynamic(true);
   for (const auto& KeyValueLookupEntry : KeyLookupTable)
   {
      const FName& Key = KeyValueLookupEntry.StateKeyName;
      if (Key.IsNone() || !Key.IsValid()) {
         continue;
      }

      switch (KeyValueLookupEntry.ExpectedValueType) {
      case EStatePropertyType::Bool:
         CopyValue(Key, this, SourceStateBlackboard->GetValueAsBool(Key));
         break;
      case EStatePropertyType::Class:
         CopyValue(Key, this, SourceStateBlackboard->GetValueAsClass(Key));
         break;
      case EStatePropertyType::Enum:
         check(false || "not supported");
         break;
      case EStatePropertyType::Float:
         CopyValue(Key, this, SourceStateBlackboard->GetValueAsFloat(Key));
         break;
      case EStatePropertyType::Int:
         CopyValue(Key, this, SourceStateBlackboard->GetValueAsInt(Key));
         break;
      case EStatePropertyType::Name:
         CopyValue(Key, this, SourceStateBlackboard->GetValueAsName(Key));
         break;
      case EStatePropertyType::Object:
         CopyValue(Key, this, SourceStateBlackboard->GetValueAsObject(Key));
         break;
      case EStatePropertyType::Rotator:
         CopyValue(Key, this, SourceStateBlackboard->GetValueAsRotator(Key));
         break;
      case EStatePropertyType::String:
         CopyValue(Key, this, SourceStateBlackboard->GetValueAsString(Key));
         break;
      case EStatePropertyType::Vector:
         CopyValue(Key, this, SourceStateBlackboard->GetValueAsVector(Key));
         break;
      case EStatePropertyType::Invalid:
      default:
         continue;
      }
      ++TotalRead;
   }
   SetAsDynamic(false);
   return TotalRead;
}