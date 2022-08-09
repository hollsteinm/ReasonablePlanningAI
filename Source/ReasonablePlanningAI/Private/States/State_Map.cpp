// Troll Purse. All rights reserved.


#include "States/State_Map.h"
#include "Core/ReasonablePlanningResourceCollection.h"

template<typename TValue>
FORCEINLINE bool TryGetFrom(const TMap<FName, TValue>& Source, FName Key, TValue& OutValue)
{
	auto Value = Source.Find(Key);
	if (Value != nullptr)
	{
		OutValue = *Value;
		return true;
	}
	return false;
}

template<typename TValue>
FORCEINLINE void TrySetMaybeAdd(TMap<FName, TValue>& Source, const FName& Key, TValue Value, bool bAllowAdditions)
{
	if (bAllowAdditions)
	{
		auto& StoredValue = Source.FindOrAdd(Key);
		StoredValue = Value;
	}
	else
	{
		auto StoredValue = Source.Find(Key);
		if (StoredValue != nullptr)
		{
			*StoredValue = Value;
		}
	}
}

UState_Map::UState_Map()
	: bDynamicMapPairs(true)
	, Resources(CreateDefaultSubobject<UReasonablePlanningResourceCollection>(TEXT("ResourceLocker")))
{

}

void UState_Map::SetBool(FName ValueName, bool Value)
{
	TrySetMaybeAdd(BoolValues, ValueName, Value, bDynamicMapPairs);
}

void UState_Map::SetClassValue(FName ValueName, UClass* Value)
{
	TrySetMaybeAdd(ClassValues, ValueName, Value, bDynamicMapPairs);
}

void UState_Map::SetEnum(FName ValueName, UEnum* Type, uint8 Value)
{
	if (Type != nullptr && Type->IsValidEnumValue(Value))
	{
		TrySetMaybeAdd(EnumValues, ValueName, Value, bDynamicMapPairs);
	}
}

void UState_Map::SetFloat(FName ValueName, float Value)
{
	TrySetMaybeAdd(FloatValues, ValueName, Value, bDynamicMapPairs);
}

void UState_Map::SetInt(FName ValueName, int32 Value)
{
	TrySetMaybeAdd(IntValues, ValueName, Value, bDynamicMapPairs);
}

void UState_Map::SetNameValue(FName ValueName, FName Value)
{
	TrySetMaybeAdd(NameValues, ValueName, Value, bDynamicMapPairs);
}

void UState_Map::SetObject(FName ValueName, UObject* Value)
{
	TrySetMaybeAdd(ObjectValues, ValueName, Value, bDynamicMapPairs);
}

void UState_Map::SetRotator(FName ValueName, FRotator Value)
{
	TrySetMaybeAdd(RotatorValues, ValueName, Value, bDynamicMapPairs);
}

void UState_Map::SetString(FName ValueName, FString Value)
{
	TrySetMaybeAdd(StringValues, ValueName, Value, bDynamicMapPairs);
}

void UState_Map::SetVector(FName ValueName, FVector Value)
{
	TrySetMaybeAdd(VectorValues, ValueName, Value, bDynamicMapPairs);
}

bool UState_Map::GetBool(FName ValueName, bool& OutValue) const
{
	return TryGetFrom(BoolValues, ValueName, OutValue);
}

bool UState_Map::GetClassValue(FName ValueName, UClass*& OutValue) const
{
	return TryGetFrom(ClassValues, ValueName, OutValue);
}

bool UState_Map::GetEnum(FName ValueName, UEnum* Type, uint8& OutValue) const
{
	if (TryGetFrom(EnumValues, ValueName, OutValue))
	{
		return Type != nullptr && Type->IsValidEnumValue(OutValue);
	}
	return false;
}

bool UState_Map::GetFloat(FName ValueName, float& OutValue) const
{
	return TryGetFrom(FloatValues, ValueName, OutValue);
}

bool UState_Map::GetInt(FName ValueName, int32& OutValue) const
{
	return TryGetFrom(IntValues, ValueName, OutValue);
}

bool UState_Map::GetNameValue(FName ValueName, FName& OutValue) const
{
	return TryGetFrom(NameValues, ValueName, OutValue);
}

bool UState_Map::GetObject(FName ValueName, UObject*& OutValue) const
{
	return TryGetFrom(ObjectValues, ValueName, OutValue);
}

bool UState_Map::GetRotator(FName ValueName, FRotator& OutValue) const
{
	return TryGetFrom(RotatorValues, ValueName, OutValue);
}

bool UState_Map::GetString(FName ValueName, FString& OutValue) const
{
	return TryGetFrom(StringValues, ValueName, OutValue);
}

bool UState_Map::GetVector(FName ValueName, FVector& OutValue) const
{
	return TryGetFrom(VectorValues, ValueName, OutValue);
}

bool UState_Map::HasBool(FName ValueName) const
{
	return BoolValues.Find(ValueName) != nullptr;
}

bool UState_Map::HasClass(FName ValueName) const
{
	return ClassValues.Find(ValueName) != nullptr;
}

bool UState_Map::HasEnum(FName ValueName) const
{
	return EnumValues.Find(ValueName) != nullptr;
}

bool UState_Map::HasFloat(FName ValueName) const
{
	return FloatValues.Find(ValueName) != nullptr;
}

bool UState_Map::HasInt(FName ValueName) const
{
	return IntValues.Find(ValueName) != nullptr;
}

bool UState_Map::HasName(FName ValueName) const
{
	return NameValues.Find(ValueName) != nullptr;
}

bool UState_Map::HasObject(FName ValueName) const
{
	return ObjectValues.Find(ValueName) != nullptr;
}

bool UState_Map::HasRotator(FName ValueName) const
{
	return RotatorValues.Find(ValueName) != nullptr;
}

bool UState_Map::HasString(FName ValueName) const
{
	return StringValues.Find(ValueName) != nullptr;
}

bool UState_Map::HasVector(FName ValueName) const
{
	return VectorValues.Find(ValueName) != nullptr;
}

bool UState_Map::HasValueWithName(FName ValueName) const
{
	return HasBool(ValueName) ||
		HasClass(ValueName) ||
		HasEnum(ValueName) ||
		HasFloat(ValueName) ||
		HasInt(ValueName) ||
		HasName(ValueName) ||
		HasObject(ValueName) ||
		HasRotator(ValueName) ||
		HasString(ValueName) ||
		HasVector(ValueName);
}

EStatePropertyType UState_Map::GetTypeFromName(FName ValueName) const
{
	if (HasBool(ValueName))
	{
		return EStatePropertyType::Bool;
	}
	if (HasClass(ValueName))
	{
		return EStatePropertyType::Class;
	}
	if (HasEnum(ValueName))
	{
		return EStatePropertyType::Enum;
	}
	if (HasFloat(ValueName))
	{
		return EStatePropertyType::Float;
	}
	if (HasInt(ValueName))
	{
		return EStatePropertyType::Int;
	}
	if (HasName(ValueName))
	{
		return EStatePropertyType::Name;
	}
	if (HasObject(ValueName))
	{
		return EStatePropertyType::Object;
	}
	if (HasRotator(ValueName))
	{
		return EStatePropertyType::Rotator;
	}
	if (HasString(ValueName))
	{
		return EStatePropertyType::String;
	}
	if (HasVector(ValueName))
	{
		return EStatePropertyType::Vector;
	}
	else
	{
		return EStatePropertyType::Invalid;
	}
}

void UState_Map::CopyStateForPredictionTo(UReasonablePlanningState* OtherState) const
{
	//TODO: Determine if we want different behavior for bDynamicMapPairs
	UState_Map* Other = Cast<UState_Map>(OtherState);
	if (Other != nullptr)
	{
		Other->BoolValues = BoolValues;
		Other->ClassValues = ClassValues;
		Other->EnumValues = EnumValues;
		Other->FloatValues = FloatValues;
		Other->IntValues = IntValues;
		Other->NameValues = NameValues;
		Other->ObjectValues = ObjectValues;
		Other->RotatorValues = RotatorValues;
		Other->StringValues = StringValues;
		Other->VectorValues = VectorValues;
	}
}

bool UState_Map::IsResourceLocked(FName ExactName) const
{
	return Resources->IsResourceLocked(ExactName);
}

bool UState_Map::IsResourceFree(FName ExactName) const
{
	return Resources->IsResourceFree(ExactName);
}

bool UState_Map::DoesHeirarchyHaveAnyLocks(FName PathPrefix) const
{
	return Resources->DoesHeirarchyHaveAnyLocks(PathPrefix);
}

bool UState_Map::LockResource(FName ExactName, UObject* OptionalLockingObject)
{
	return Resources->LockResource(OptionalLockingObject == nullptr ? this : OptionalLockingObject, ExactName);
}

bool UState_Map::UnlockResource(FName ExactName, UObject* OptionalLockingObject)
{
	return Resources->UnlockResource(OptionalLockingObject == nullptr ? this : OptionalLockingObject, ExactName);
}
