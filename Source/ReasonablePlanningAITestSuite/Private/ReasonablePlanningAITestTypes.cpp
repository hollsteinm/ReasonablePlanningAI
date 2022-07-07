#include "ReasonablePlanningAITestTypes.h"

FName UTestPlanningState::NAME_TheBoolValue = "TheBoolValue";
FName UTestPlanningState::NAME_TheClassValue = "TheClassValue";
FName UTestPlanningState::NAME_TheTestEnumValue = "TheTestEnumValue";
FName UTestPlanningState::NAME_TheFloatValue = "TheFloatValue";
FName UTestPlanningState::NAME_TheIntValue = "TheIntValue";
FName UTestPlanningState::NAME_TheNameValue = "TheNameValue";
FName UTestPlanningState::NAME_TheObjectValue = "TheObjectValue";
FName UTestPlanningState::NAME_TheRotatorValue = "TheRotatorValue";
FName UTestPlanningState::NAME_TheStringValue = "TheStringValue";
FName UTestPlanningState::NAME_TheVectorValue = "TheVectorValue";

void UTestPlanningState::SetBool(FName ValueName, bool Value)
{
	if (ValueName.IsEqual(NAME_TheBoolValue))
	{
		TheBoolValue = Value;
	}
}

void UTestPlanningState::SetClassValue(FName ValueName, UClass* Value)
{
	if (ValueName.IsEqual(NAME_TheClassValue))
	{
		TheClassValue = Value;
	}
}

void UTestPlanningState::SetEnum(FName ValueName, UEnum* Type, uint8 Value)
{
	if (ValueName.IsEqual(NAME_TheTestEnumValue))
	{
		TheTestEnumValue = static_cast<ETestEnum>(Value);
	}
}

void UTestPlanningState::SetFloat(FName ValueName, float Value)
{
	if (ValueName.IsEqual(NAME_TheFloatValue))
	{
		TheFloatValue = Value;
	}
}

void UTestPlanningState::SetInt(FName ValueName, int32 Value)
{
	if (ValueName.IsEqual(NAME_TheIntValue))
	{
		TheIntValue = Value;
	}
}

void UTestPlanningState::SetNameValue(FName ValueName, FName Value)
{
	if (ValueName.IsEqual(NAME_TheNameValue))
	{
		TheNameValue = Value;
	}
}

void UTestPlanningState::SetObject(FName ValueName, UObject* Value)
{
	if (ValueName.IsEqual(NAME_TheObjectValue))
	{
		TheObjectValue = Value;
	}
}

void UTestPlanningState::SetRotator(FName ValueName, FRotator Value)
{
	if (ValueName.IsEqual(NAME_TheRotatorValue))
	{
		TheRotatorValue = Value;
	}
}

void UTestPlanningState::SetString(FName ValueName, FString Value)
{
	if (ValueName.IsEqual(NAME_TheStringValue))
	{
		TheStringValue = Value;
	}
}

void UTestPlanningState::SetVector(FName ValueName, FVector Value)
{
	if (ValueName.IsEqual(NAME_TheVectorValue))
	{
		TheVectorValue = Value;
	}
}

bool UTestPlanningState::GetBool(FName ValueName, bool& OutValue) const
{
	if (ValueName.IsEqual(NAME_TheBoolValue))
	{
		OutValue = TheBoolValue;
		return true;
	}
	return false;
}

bool UTestPlanningState::GetClassValue(FName ValueName, UClass*& OutValue) const
{
	if (ValueName.IsEqual(NAME_TheClassValue))
	{
		OutValue = TheClassValue;
		return true;
	}
	return false;
}

bool UTestPlanningState::GetEnum(FName ValueName, UEnum* Type, uint8& OutValue) const
{
	if (ValueName.IsEqual(NAME_TheTestEnumValue))
	{
		OutValue = static_cast<uint8>(TheTestEnumValue);
		return true;
	}
	return false;
}

bool UTestPlanningState::GetFloat(FName ValueName, float& OutValue) const
{
	if (ValueName.IsEqual(NAME_TheFloatValue))
	{
		OutValue = TheFloatValue;
		return true;
	}
	return false;
}

bool UTestPlanningState::GetInt(FName ValueName, int32& OutValue) const
{
	if (ValueName.IsEqual(NAME_TheIntValue))
	{
		OutValue = TheIntValue;
		return true;
	}
	return false;
}

bool UTestPlanningState::GetNameValue(FName ValueName, FName& OutValue) const
{
	if (ValueName.IsEqual(NAME_TheNameValue))
	{
		OutValue = TheNameValue;
		return true;
	}
	return false;
}

bool UTestPlanningState::GetObject(FName ValueName, UObject*& OutValue) const
{
	if (ValueName.IsEqual(NAME_TheObjectValue))
	{
		OutValue = TheObjectValue;
		return true;
	}
	return false;
}

bool UTestPlanningState::GetRotator(FName ValueName, FRotator& OutValue) const
{
	if (ValueName.IsEqual(NAME_TheRotatorValue))
	{
		OutValue = TheRotatorValue;
		return true;
	}
	return false;
}

bool UTestPlanningState::GetString(FName ValueName, FString& OutValue) const
{
	if (ValueName.IsEqual(NAME_TheStringValue))
	{
		OutValue = TheStringValue;
		return true;
	}
	return false;
}

bool UTestPlanningState::GetVector(FName ValueName, FVector& OutValue) const
{
	if (ValueName.IsEqual(NAME_TheVectorValue))
	{
		OutValue = TheVectorValue;
		return true;
	}
	return false;
}

bool UTestPlanningState::HasBool(FName ValueName) const
{
	return ValueName.IsEqual(NAME_TheBoolValue);
}

bool UTestPlanningState::HasClass(FName ValueName) const
{
	return ValueName.IsEqual(NAME_TheClassValue);
}

bool UTestPlanningState::HasEnum(FName ValueName) const
{
	return ValueName.IsEqual(NAME_TheTestEnumValue);
}

bool UTestPlanningState::HasFloat(FName ValueName) const
{
	return ValueName.IsEqual(NAME_TheFloatValue);
}

bool UTestPlanningState::HasInt(FName ValueName) const
{
	return ValueName.IsEqual(NAME_TheIntValue);
}

bool UTestPlanningState::HasName(FName ValueName) const
{
	return ValueName.IsEqual(NAME_TheNameValue);
}

bool UTestPlanningState::HasObject(FName ValueName) const
{
	return ValueName.IsEqual(NAME_TheObjectValue);
}

bool UTestPlanningState::HasRotator(FName ValueName) const
{
	return ValueName.IsEqual(NAME_TheRotatorValue);
}

bool UTestPlanningState::HasString(FName ValueName) const
{
	return ValueName.IsEqual(NAME_TheStringValue);
}

bool UTestPlanningState::HasVector(FName ValueName) const
{
	return ValueName.IsEqual(NAME_TheVectorValue);
}

bool UTestPlanningState::HasValueWithName(FName ValueName) const
{
	if (ValueName.IsEqual(NAME_TheBoolValue)) { return true; }
	else if(ValueName.IsEqual(NAME_TheClassValue)) { return true; }
	else if(ValueName.IsEqual(NAME_TheTestEnumValue)) { return true; }
	else if(ValueName.IsEqual(NAME_TheFloatValue)) { return true; }
	else if(ValueName.IsEqual(NAME_TheIntValue)) { return true; }
	else if(ValueName.IsEqual(NAME_TheNameValue)) { return true; }
	else if(ValueName.IsEqual(NAME_TheObjectValue)) { return true; }
	else if(ValueName.IsEqual(NAME_TheRotatorValue)) { return true; }
	else if(ValueName.IsEqual(NAME_TheStringValue)) { return true; }
	else if(ValueName.IsEqual(NAME_TheVectorValue)) { return true; }
	else { return false; }
}

EStatePropertyType UTestPlanningState::GetTypeFromName(FName ValueName) const
{
	if (ValueName.IsEqual(NAME_TheBoolValue)) { return EStatePropertyType::Bool; }
	else if (ValueName.IsEqual(NAME_TheClassValue)) { return EStatePropertyType::Class; }
	else if (ValueName.IsEqual(NAME_TheTestEnumValue)) { return EStatePropertyType::Enum; }
	else if (ValueName.IsEqual(NAME_TheFloatValue)) { return EStatePropertyType::Float; }
	else if (ValueName.IsEqual(NAME_TheIntValue)) { return EStatePropertyType::Int; }
	else if (ValueName.IsEqual(NAME_TheNameValue)) { return EStatePropertyType::Name; }
	else if (ValueName.IsEqual(NAME_TheObjectValue)) { return EStatePropertyType::Object; }
	else if (ValueName.IsEqual(NAME_TheRotatorValue)) { return EStatePropertyType::Rotator; }
	else if (ValueName.IsEqual(NAME_TheStringValue)) { return EStatePropertyType::String; }
	else if (ValueName.IsEqual(NAME_TheVectorValue)) { return EStatePropertyType::Vector; }
	else { return EStatePropertyType::Invalid; }
}

void UTestPlanningState::CopyStateForPredictionTo(UReasonablePlanningState* OtherState) const
{
	UTestPlanningState* OtherTestState = Cast<UTestPlanningState>(OtherState);
	check(OtherTestState != nullptr);

	OtherTestState->TheBoolValue = TheBoolValue;
	OtherTestState->TheClassValue = TheClassValue;
	OtherTestState->TheTestEnumValue = TheTestEnumValue;
	OtherTestState->TheFloatValue = TheFloatValue;
	OtherTestState->TheIntValue = TheIntValue;
	OtherTestState->TheNameValue = TheNameValue;
	OtherTestState->TheObjectValue = TheObjectValue;
	OtherTestState->TheRotatorValue = TheRotatorValue;
	OtherTestState->TheStringValue = TheStringValue;
	OtherTestState->TheVectorValue = TheVectorValue;
}

UTestAction::UTestAction()
	: Weight(1.f)
	, KeyToApply("Test")
	, ValueToApply(1)
	, IsApplicable(true)
{

}

void UTestAction::ReceiveApplyToState_Implementation(UReasonablePlanningState* GivenState) const
{
	GivenState->SetInt(KeyToApply, ValueToApply);
}

bool UTestAction::ReceiveIsApplicable_Implementation(const UReasonablePlanningState* GivenState) const
{
	if (IsApplicable)
	{
		int32 CurrentValue = 0;
		if (GivenState->GetValueOfType(KeyToApply, CurrentValue))
		{
			return CurrentValue < ValueToApply;
		}
	}
	return false;
}

float UTestAction::ReceiveExecutionWeight_Implementation(const UReasonablePlanningState* GivenState) const
{
	return Weight;
}

bool UTestGoal::ReceiveIsInDesiredState_Implementation(const UReasonablePlanningState* GivenState) const
{
	int32 StateAValue = 0;
	int32 StateBValue = 0;
	int32 StateCValue = 0;
	if (GivenState->GetValueOfType(ConditionAKey, StateAValue) && GivenState->GetValueOfType(ConditionBKey, StateBValue) && GivenState->GetValueOfType(ConditionCKey, StateCValue))
	{
		return StateAValue >= ConditionAValue && StateBValue >= ConditionBValue && StateCValue >= ConditionCValue;
	}
	return false;
}

//Unordered distance
float UTestGoal::ReceiveGetDistanceToDesiredState_Implementation(const UReasonablePlanningState* GivenState) const
{
	int32 StateAValue = 0;
	int32 StateBValue = 0;
	int32 StateCValue = 0;
	if (GivenState->GetValueOfType(ConditionAKey, StateAValue) && GivenState->GetValueOfType(ConditionBKey, StateBValue) && GivenState->GetValueOfType(ConditionCKey, StateCValue))
	{
		return FMath::Min(0, ConditionAValue - StateAValue) + FMath::Min(0, ConditionBValue - StateCValue) + FMath::Min(0, ConditionCValue - StateCValue);
	}
	return TNumericLimits<float>::Max();
}

const FName UTestGoal::ConditionAKey = "TestA";
const FName UTestGoal::ConditionBKey = "TestB";
const FName UTestGoal::ConditionCKey = "TestC";