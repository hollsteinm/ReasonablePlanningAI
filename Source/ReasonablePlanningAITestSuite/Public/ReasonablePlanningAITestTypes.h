#pragma once

#include "Core/RpaiState.h"
#include "Core/RpaiActionBase.h"
#include "Core/RpaiGoalBase.h"
#include "ReasonablePlanningAITestTypes.generated.h"

UENUM()
enum class ETestEnum : uint8
{
	A,
	B,
	C
};

UCLASS(HideDropdown)
class UTestPlanningState : public URpaiState
{
	GENERATED_BODY()
private:
	bool TheBoolValue;
	UClass* TheClassValue;
	ETestEnum TheTestEnumValue;
	float TheFloatValue;
	int32 TheIntValue;
	FName TheNameValue;
	UObject* TheObjectValue;
	FRotator TheRotatorValue;
	FString TheStringValue;
	FVector TheVectorValue;

public:
	static FName NAME_TheBoolValue;
	static FName NAME_TheClassValue;
	static FName NAME_TheTestEnumValue;
	static FName NAME_TheFloatValue;
	static FName NAME_TheIntValue;
	static FName NAME_TheNameValue;
	static FName NAME_TheObjectValue;
	static FName NAME_TheRotatorValue;
	static FName NAME_TheStringValue;
	static FName NAME_TheVectorValue;

	virtual void SetBool(FName ValueName, bool Value) override;
	virtual void SetClassValue(FName ValueName, UClass* Value) override;
	virtual void SetEnum(FName ValueName, UEnum* Type, uint8 Value) override;
	virtual void SetFloat(FName ValueName, float Value) override;
	virtual void SetInt(FName ValueName, int32 Value) override;
	virtual void SetNameValue(FName ValueName, FName Value) override;
	virtual void SetObject(FName ValueName, UObject* Value) override;
	virtual void SetRotator(FName ValueName, FRotator Value) override;
	virtual void SetString(FName ValueName, FString Value) override;
	virtual void SetVector(FName ValueName, FVector Value) override;
	virtual bool GetBool(FName ValueName, bool& OutValue) const override;
	virtual bool GetClassValue(FName ValueName, UClass * &OutValue) const override;
	virtual bool GetEnum(FName ValueName, UEnum * Type, uint8 & OutValue) const override;
	virtual bool GetFloat(FName ValueName, float& OutValue) const override;
	virtual bool GetInt(FName ValueName, int32 & OutValue) const override;
	virtual bool GetNameValue(FName ValueName, FName & OutValue) const override;
	virtual bool GetObject(FName ValueName, UObject * &OutValue) const override;
	virtual bool GetRotator(FName ValueName, FRotator & OutValue) const override;
	virtual bool GetString(FName ValueName, FString & OutValue) const override;
	virtual bool GetVector(FName ValueName, FVector & OutValue) const override;
	virtual bool HasBool(FName ValueName) const override;
	virtual bool HasClass(FName ValueName) const override;
	virtual bool HasEnum(FName ValueName) const override;
	virtual bool HasFloat(FName ValueName) const override;
	virtual bool HasInt(FName ValueName) const override;
	virtual bool HasName(FName ValueName) const override;
	virtual bool HasObject(FName ValueName) const override;
	virtual bool HasRotator(FName ValueName) const override;
	virtual bool HasString(FName ValueName) const override;
	virtual bool HasVector(FName ValueName) const override;
	virtual bool HasValueWithName(FName ValueName) const override;
	virtual EStatePropertyType GetTypeFromName(FName ValueName) const override;
	virtual void CopyStateForPredictionTo(URpaiState* OtherState) const override;
};

UCLASS(HideDropdown)
class UTestAction : public URpaiActionBase
{
	GENERATED_BODY()

public:
	float Weight;
	FName KeyToApply;
	int32 ValueToApply;
	bool IsApplicable;

	UTestAction();

protected:
	//Only implement the planning heuristic functions

	virtual void ReceiveApplyToState_Implementation(URpaiState* GivenState) const override;
	virtual bool ReceiveIsApplicable_Implementation(const URpaiState* GivenState) const override;
	virtual float ReceiveExecutionWeight_Implementation(const URpaiState* GivenState) const override;
};

UCLASS(HideDropdown)
class UTestGoal : public URpaiGoalBase
{
	GENERATED_BODY()

public:
	UTestGoal();

	int32 ConditionAValue;
	int32 ConditionBValue;
	int32 ConditionCValue;
	static const FName ConditionAKey;
	static const FName ConditionBKey;
	static const FName ConditionCKey;

	float Weight;

	virtual bool ReceiveIsInDesiredState_Implementation(const URpaiState* GivenState) const override;
	virtual float ReceiveGetDistanceToDesiredState_Implementation(const URpaiState* GivenState) const override;
	virtual float ReceiveGetWeight_Implementation(const URpaiState* GivenState) const override;
	virtual bool ReceiveIsApplicable_Implementation(const URpaiState* GivenState) const override;
};