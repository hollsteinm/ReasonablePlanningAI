// Troll Purse. All rights reserved.

#include "States/RpaiState_Reflection.h"
#include "Core/RpaiResourceCollection.h"
#include "PropertyPathHelpers.h"
#include "UObject/UnrealType.h"

template<typename TType>
static bool GetStructType(const URpaiState_Reflection* Container, FName ValueName, TType& OutValue)
{
	check(Container != nullptr);
	using DataType = TDecay<TType>::Type;
	auto Property = CastField<FStructProperty>(Container->GetClass()->FindPropertyByName(ValueName));
	if (Property != nullptr && Property->Struct == TBaseStructure<DataType>::Get())
	{
		auto PropertyValuePtr = Property->ContainerPtrToValuePtr<TType>(Container);
		if (PropertyValuePtr != nullptr)
		{
			OutValue = *PropertyValuePtr;
			return true;
		}
	}
	return false;
}

template<typename TType>
static void SetStructType(URpaiState_Reflection* Container, FName ValueName, TType Value)
{
	check(Container != nullptr);
	using DataType = TDecay<TType>::Type;
	auto Property = CastField<FStructProperty>(Container->GetClass()->FindPropertyByName(ValueName));
	if (Property != nullptr && Property->Struct == TBaseStructure<DataType>::Get())
	{
		auto PropertyValuePtr = Property->ContainerPtrToValuePtr<TType>(Container);
		if (PropertyValuePtr != nullptr)
		{
			FMemory::Memcpy(PropertyValuePtr, &Value, sizeof(TType));
		}
	}
}

URpaiState_Reflection::URpaiState_Reflection()
	: Resources(CreateDefaultSubobject<URpaiResourceCollection>(TEXT("Resources")))
{
}

bool URpaiState_Reflection::IsEqualTo(const URpaiState* OtherState) const
{
	check(OtherState != nullptr);
	if (OtherState == this)
	{
		return true;
	}
	if (!OtherState->IsA<URpaiState_Reflection>())
	{
		return false;
	}

	for (TFieldIterator<FProperty> Piter(GetClass(), EFieldIterationFlags::IncludeDeprecated); Piter; ++Piter)
	{
		if (Piter->IsA<FBoolProperty>())
		{
			bool Rhs;
			if (OtherState->GetValueOfType(Piter->NamePrivate, Rhs))
			{
				bool Lhs = CastField<FBoolProperty>(*Piter)->GetPropertyValue_InContainer(this);
				if (Lhs != Rhs)
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		else if (Piter->IsA<FClassProperty>())
		{
			UClass* Rhs;
			if (OtherState->GetValueOfType(Piter->NamePrivate, Rhs))
			{
				UClass* Lhs = Cast<UClass>(CastField<FClassProperty>(*Piter)->GetPropertyValue_InContainer(this));
				if (Lhs != Rhs)
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		else if (Piter->IsA<FEnumProperty>())
		{
			auto EnumProperty = CastField<FEnumProperty>(*Piter);
			
			uint8 Rhs;
			if (OtherState->GetEnum(Piter->NamePrivate, EnumProperty->GetEnum(), Rhs))
			{
				if (EnumProperty->GetUnderlyingProperty()->IsA<FByteProperty>())
				{
					auto ByteProperty = CastField<FByteProperty>(EnumProperty->GetUnderlyingProperty());
					uint8 Lhs = ByteProperty->GetPropertyValue_InContainer(this);
					if (Lhs != Rhs)
					{
						return false;
					}
				}
			}
			else
			{
				return false;
			}
		}
		else if (Piter->IsA<FFloatProperty>())
		{
			float Rhs;
			if (OtherState->GetValueOfType(Piter->NamePrivate, Rhs))
			{
				float Lhs = CastField<FFloatProperty>(*Piter)->GetPropertyValue_InContainer(this);
				if (Lhs != Rhs)
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		else if (Piter->IsA<FDoubleProperty>())
		{
			double Rhs;
			if (OtherState->GetValueOfType(Piter->NamePrivate, Rhs))
			{
				double Lhs = CastField<FDoubleProperty>(*Piter)->GetPropertyValue_InContainer(this);
				if (Lhs != Rhs)
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		else if (Piter->IsA<FIntProperty>())
		{
			int32 Rhs;
			if (OtherState->GetValueOfType(Piter->NamePrivate, Rhs))
			{
				int32 Lhs = CastField<FIntProperty>(*Piter)->GetPropertyValue_InContainer(this);
				if (Lhs != Rhs)
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		else if (Piter->IsA<FNameProperty>())
		{
			FName Rhs;
			if (OtherState->GetValueOfType(Piter->NamePrivate, Rhs))
			{
				FName Lhs = CastField<FNameProperty>(*Piter)->GetPropertyValue_InContainer(this);
				if (Lhs != Rhs)
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		else if (Piter->IsA<FStrProperty>())
		{
			FString Rhs;
			if (OtherState->GetValueOfType(Piter->NamePrivate, Rhs))
			{
				FString Lhs = CastField<FStrProperty>(*Piter)->GetPropertyValue_InContainer(this);
				if (Lhs != Rhs)
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		else if (Piter->IsA<FObjectProperty>())
		{
			UObject* Rhs;
			if (OtherState->GetValueOfType(Piter->NamePrivate, Rhs))
			{
				UObject* Lhs = CastField<FObjectProperty>(*Piter)->GetPropertyValue_InContainer(this);
				if (Lhs != Rhs)
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		else if (Piter->IsA<FStructProperty>())
		{
			if (auto StructProperty = CastField<FStructProperty>(*Piter))
			{
				if (StructProperty->Struct == TBaseStructure<FVector>::Get())
				{
					FVector Rhs;
					if (OtherState->GetValueOfType(Piter->NamePrivate, Rhs))
					{
						if (auto LhsPtr = StructProperty->ContainerPtrToValuePtr<FVector>(this))
						{
							if ((*LhsPtr) != Rhs)
							{
								return false;
							}
						}
						else
						{
							return false;
						}
					}
					else
					{
						return false;
					}
				}
				else if (StructProperty->Struct == TBaseStructure<FRotator>::Get())
				{
					FRotator Rhs;
					if (OtherState->GetValueOfType(Piter->NamePrivate, Rhs))
					{
						if (auto LhsPtr = StructProperty->ContainerPtrToValuePtr<FRotator>(this))
						{
							if ((*LhsPtr) != Rhs)
							{
								return false;
							}
						}
						else
						{
							return false;
						}
					}
					else
					{
						return false;
					}
				}
			}
		}
	}
	return true;
}

void URpaiState_Reflection::SetBool(FName ValueName, bool Value)
{
	auto Property = FindFProperty<FBoolProperty>(GetClass(), ValueName);
	if (Property != nullptr)
	{
		Property->SetPropertyValue_InContainer(this, Value);
	}
}

void URpaiState_Reflection::SetClassValue(FName ValueName, UClass* Value)
{
	auto Property = FindFProperty<FClassProperty>(GetClass(), ValueName);
	if (Property != nullptr)
	{
		Property->SetPropertyValue_InContainer(this, Value);
	}
}

void URpaiState_Reflection::SetEnum(FName ValueName, UEnum* Type, uint8 Value)
{
	if (Type != nullptr && Type->IsValidEnumValue(Value))
	{
		auto Property = FindFProperty<FEnumProperty>(GetClass(), ValueName);
		if (Property != nullptr)
		{
			if (Property->GetEnum() == Type)
			{
				auto UnderlyingProperty = CastField<FByteProperty>(Property->GetUnderlyingProperty());
				if (UnderlyingProperty != nullptr)
				{
					UnderlyingProperty->SetPropertyValue_InContainer(this, Value);
				}
			}
		}
	}
}

void URpaiState_Reflection::SetFloat(FName ValueName, float Value)
{
	auto Property = FindFProperty<FProperty>(GetClass(), ValueName);
	if (auto FloatProperty = CastField<FFloatProperty>(Property))
	{
		FloatProperty->SetPropertyValue_InContainer(this, Value);
	}
	else if (auto DoubleProperty = CastField<FDoubleProperty>(Property))
	{
		DoubleProperty->SetPropertyValue_InContainer(this, static_cast<double>(Value));
	}
}

void URpaiState_Reflection::SetInt(FName ValueName, int32 Value)
{
	auto Property = FindFProperty<FIntProperty>(GetClass(), ValueName);
	if (Property != nullptr)
	{
		Property->SetPropertyValue_InContainer(this, Value);
	}
}

void URpaiState_Reflection::SetNameValue(FName ValueName, FName Value)
{
	auto Property = FindFProperty<FNameProperty>(GetClass(), ValueName);
	if (Property != nullptr)
	{
		Property->SetPropertyValue_InContainer(this, Value);
	}
}

void URpaiState_Reflection::SetObject(FName ValueName, UObject* Value)
{
	auto Property = FindFProperty<FObjectProperty>(GetClass(), ValueName);
	if (Property != nullptr)
	{
		Property->SetPropertyValue_InContainer(this, Value);
	}
}

void URpaiState_Reflection::SetRotator(FName ValueName, FRotator Value)
{
	SetStructType(this, ValueName, Value);
}

void URpaiState_Reflection::SetString(FName ValueName, FString Value)
{
	auto Property = FindFProperty<FStrProperty>(GetClass(), ValueName);
	if (Property != nullptr)
	{
		Property->SetPropertyValue_InContainer(this, Value);
	}
}

void URpaiState_Reflection::SetVector(FName ValueName, FVector Value)
{
	SetStructType(this, ValueName, Value);
}

bool URpaiState_Reflection::GetBool(FName ValueName, bool& OutValue) const
{
	auto Property = FindFProperty<FBoolProperty>(GetClass(), ValueName);
	if (Property != nullptr)
	{
		OutValue = Property->GetPropertyValue_InContainer(this);
		return true;
	}
	return false;
}

bool URpaiState_Reflection::GetClassValue(FName ValueName, UClass*& OutValue) const
{
	auto Property = FindFProperty<FClassProperty>(GetClass(), ValueName);
	if (Property != nullptr)
	{
		OutValue = Cast<UClass>(Property->GetPropertyValue_InContainer(this));
		return true;
	}
	return false;
}

bool URpaiState_Reflection::GetEnum(FName ValueName, UEnum* Type, uint8& OutValue) const
{
	auto Property = FindFProperty<FEnumProperty>(GetClass(), ValueName);
	if (Property != nullptr)
	{
		if (Property->GetEnum() == Type)
		{
			auto UnderlyingProperty = CastField<FByteProperty>(Property->GetUnderlyingProperty());
			if (UnderlyingProperty != nullptr)
			{
				OutValue = UnderlyingProperty->GetPropertyValue_InContainer(this);
				return true;
			}
		}
	}
	return false;
}

bool URpaiState_Reflection::GetFloat(FName ValueName, float& OutValue) const
{
	auto Property = FindFProperty<FProperty>(GetClass(), ValueName);
	if (auto FloatProperty = CastField<FFloatProperty>(Property))
	{
		OutValue = FloatProperty->GetPropertyValue_InContainer(this);
		return true;
	}
	else if (auto DoubleProperty = CastField<FDoubleProperty>(Property))
	{
		OutValue = DoubleProperty->GetPropertyValue_InContainer(this);
		return true;
	}
	return false;
}

bool URpaiState_Reflection::GetInt(FName ValueName, int32& OutValue) const
{
	auto Property = FindFProperty<FIntProperty>(GetClass(), ValueName);
	if (Property != nullptr)
	{
		OutValue = Property->GetPropertyValue_InContainer(this);
		return true;
	}
	return false;
}

bool URpaiState_Reflection::GetNameValue(FName ValueName, FName& OutValue) const
{
	auto Property = FindFProperty<FNameProperty>(GetClass(), ValueName);
	if (Property != nullptr)
	{
		OutValue = Property->GetPropertyValue_InContainer(this);
		return true;
	}
	return false;
}

bool URpaiState_Reflection::GetObject(FName ValueName, UObject*& OutValue) const
{
	auto Property = FindFProperty<FObjectProperty>(GetClass(), ValueName);
	if (Property != nullptr)
	{
		OutValue = Property->GetPropertyValue_InContainer(this);
		return true;
	}
	return false;
}

bool URpaiState_Reflection::GetRotator(FName ValueName, FRotator& OutValue) const
{
	return GetStructType(this, ValueName, OutValue);
}

bool URpaiState_Reflection::GetString(FName ValueName, FString& OutValue) const
{
	auto Property = FindFProperty<FStrProperty>(GetClass(), ValueName);
	if (Property != nullptr)
	{
		OutValue = Property->GetPropertyValue_InContainer(this);
		return true;
	}
	return false;
}

bool URpaiState_Reflection::GetVector(FName ValueName, FVector& OutValue) const
{
	return GetStructType(this, ValueName, OutValue);
}

bool URpaiState_Reflection::HasBool(FName ValueName) const
{
	auto Property = GetClass()->FindPropertyByName(ValueName);
	return Property != nullptr ? Property->IsA<FBoolProperty>() : false;
}

bool URpaiState_Reflection::HasClass(FName ValueName) const
{
	auto Property = GetClass()->FindPropertyByName(ValueName);
	return Property != nullptr ? Property->IsA<FClassProperty>() : false;
}

bool URpaiState_Reflection::HasEnum(FName ValueName) const
{
	auto Property = GetClass()->FindPropertyByName(ValueName);
	return Property != nullptr ? Property->IsA<FEnumProperty>() : false;
}

bool URpaiState_Reflection::HasFloat(FName ValueName) const
{
	auto Property = GetClass()->FindPropertyByName(ValueName);
	return Property != nullptr ? (Property->IsA<FFloatProperty>() || Property->IsA<FDoubleProperty>()) : false;
}

bool URpaiState_Reflection::HasInt(FName ValueName) const
{
	auto Property = GetClass()->FindPropertyByName(ValueName);
	return Property != nullptr ? Property->IsA<FIntProperty>() : false;
}

bool URpaiState_Reflection::HasName(FName ValueName) const
{
	auto Property = GetClass()->FindPropertyByName(ValueName);
	return Property != nullptr ? Property->IsA<FNameProperty>() : false;
}

bool URpaiState_Reflection::HasObject(FName ValueName) const
{
	auto Property = GetClass()->FindPropertyByName(ValueName);
	return Property != nullptr ? Property->IsA<FObjectProperty>() : false;
}

bool URpaiState_Reflection::HasRotator(FName ValueName) const
{
	auto Property = GetClass()->FindPropertyByName(ValueName);
	if (Property != nullptr)
	{
		auto StructProperty = CastField<FStructProperty>(Property);
		return StructProperty != nullptr ? StructProperty->Struct == TBaseStructure<FRotator>::Get() : false;
	}
	return false;
}

bool URpaiState_Reflection::HasString(FName ValueName) const
{
	auto Property = GetClass()->FindPropertyByName(ValueName);
	return Property != nullptr ? Property->IsA<FStrProperty>() : false;
}

bool URpaiState_Reflection::HasVector(FName ValueName) const
{
	auto Property = GetClass()->FindPropertyByName(ValueName);
	if (Property != nullptr)
	{
		auto StructProperty = CastField<FStructProperty>(Property);
		return StructProperty != nullptr ? StructProperty->Struct == TBaseStructure<FVector>::Get() : false;
	}
	return false;
}

bool URpaiState_Reflection::HasValueWithName(FName ValueName) const
{
	return GetClass()->FindPropertyByName(ValueName) != nullptr;
}

EStatePropertyType URpaiState_Reflection::GetTypeFromName(FName ValueName) const
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

void URpaiState_Reflection::CopyStateForPredictionTo(URpaiState* OtherState) const
{
	check(OtherState != nullptr);
	if (OtherState->IsA<URpaiState_Reflection>())
	{
		for (TFieldIterator<FProperty> Piter(GetClass(), EFieldIterationFlags::IncludeDeprecated); Piter; ++Piter)
		{
			if (Piter->IsA<FBoolProperty>())
			{
				OtherState->SetValueOfType(Piter->NamePrivate, CastField<FBoolProperty>(*Piter)->GetPropertyValue_InContainer(this));
			}
			else if (Piter->IsA<FClassProperty>())
			{
				OtherState->SetValueOfType(Piter->NamePrivate, Cast<UClass>(CastField<FClassProperty>(*Piter)->GetPropertyValue_InContainer(this)));
			}
			else if (Piter->IsA<FEnumProperty>())
			{
				auto EnumProperty = CastField<FEnumProperty>(*Piter);
				if(EnumProperty->GetUnderlyingProperty()->IsA<FByteProperty>())
				{
					auto ByteProperty = CastField<FByteProperty>(EnumProperty->GetUnderlyingProperty());
					OtherState->SetEnum(Piter->NamePrivate, EnumProperty->GetEnum(), ByteProperty->GetPropertyValue_InContainer(this));
				}
			}
			else if (Piter->IsA<FFloatProperty>())
			{
				OtherState->SetValueOfType(Piter->NamePrivate, CastField<FFloatProperty>(*Piter)->GetPropertyValue_InContainer(this));
			}
			else if (Piter->IsA<FDoubleProperty>())
			{
				OtherState->SetValueOfType(Piter->NamePrivate, CastField<FDoubleProperty>(*Piter)->GetPropertyValue_InContainer(this));
			}
			else if (Piter->IsA<FIntProperty>())
			{
				OtherState->SetValueOfType(Piter->NamePrivate, CastField<FIntProperty>(*Piter)->GetPropertyValue_InContainer(this));
			}
			else if (Piter->IsA<FNameProperty>())
			{
				OtherState->SetValueOfType(Piter->NamePrivate, CastField<FNameProperty>(*Piter)->GetPropertyValue_InContainer(this));
			}
			else if (Piter->IsA<FStrProperty>())
			{
				OtherState->SetValueOfType(Piter->NamePrivate, CastField<FStrProperty>(*Piter)->GetPropertyValue_InContainer(this));
			}
			else if (Piter->IsA<FObjectProperty>())
			{
				OtherState->SetValueOfType(Piter->NamePrivate, CastField<FObjectProperty>(*Piter)->GetPropertyValue_InContainer(this));
			}
			else if (Piter->IsA<FStructProperty>())
			{
				if (auto StructProperty = CastField<FStructProperty>(*Piter))
				{
					if (StructProperty->Struct == TBaseStructure<FVector>::Get())
					{
						OtherState->SetValueOfType(Piter->NamePrivate, StructProperty->ContainerPtrToValuePtr<FVector>(this));
					}
					else if (StructProperty->Struct == TBaseStructure<FRotator>::Get())
					{
						OtherState->SetValueOfType(Piter->NamePrivate, StructProperty->ContainerPtrToValuePtr<FRotator>(this));
					}
				}
			}
		}
	}
}

bool URpaiState_Reflection::IsResourceLocked(FName ExactName) const
{
	return Resources->IsResourceLocked(ExactName);
}

bool URpaiState_Reflection::IsResourceFree(FName ExactName) const
{
	return Resources->IsResourceFree(ExactName);
}

bool URpaiState_Reflection::DoesHeirarchyHaveAnyLocks(FName PathPrefix) const
{
	return Resources->DoesHeirarchyHaveAnyLocks(PathPrefix);
}

bool URpaiState_Reflection::LockResource(FName ExactName, UObject* OptionalLockingObject)
{
	return Resources->LockResource(OptionalLockingObject == nullptr ? this : OptionalLockingObject, ExactName);
}

bool URpaiState_Reflection::UnlockResource(FName ExactName, UObject* OptionalLockingObject)
{
	return Resources->UnlockResource(OptionalLockingObject == nullptr ? this : OptionalLockingObject, ExactName);
}