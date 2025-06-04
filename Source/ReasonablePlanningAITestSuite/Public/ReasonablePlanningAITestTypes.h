// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.

#pragma once

#include "Core/RpaiState.h"
#include "States/RpaiState_Reflection.h"
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
class UTestPlanningState : public URpaiState_Reflection
{
   GENERATED_BODY()
private:
   UPROPERTY()
   bool TheBoolValue;

   UPROPERTY()
   UClass* TheClassValue;

   UPROPERTY()
   ETestEnum TheTestEnumValue;

   UPROPERTY()
   float TheFloatValue;

   UPROPERTY()
   int32 TheIntValue;

   UPROPERTY()
   FName TheNameValue;

   UPROPERTY()
   UObject* TheObjectValue;

   UPROPERTY()
   FRotator TheRotatorValue;

   UPROPERTY()
   FString TheStringValue;

   UPROPERTY()
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

USTRUCT(BlueprintType)
struct FTestStruct
{
   GENERATED_BODY()

   UPROPERTY()
   int32 Value;
};


USTRUCT(BlueprintType)
struct FComplexTestStruct
{
   GENERATED_BODY()

   UPROPERTY()
   float Value;

   UPROPERTY()
   FTestStruct Inner;
};