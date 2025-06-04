// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.

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

UTestAction::UTestAction()
   : Weight(1.f)
   , KeyToApply("Test")
   , ValueToApply(1)
   , IsApplicable(true)
{

}

void UTestAction::ReceiveApplyToState_Implementation(URpaiState* GivenState) const
{
   GivenState->SetInt(KeyToApply, ValueToApply);
}

bool UTestAction::ReceiveIsApplicable_Implementation(const URpaiState* GivenState) const
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

float UTestAction::ReceiveExecutionWeight_Implementation(const URpaiState* GivenState) const
{
   return Weight;
}

UTestGoal::UTestGoal()
   : ConditionAValue(2)
   , ConditionBValue(1)
   , ConditionCValue(3)
   , Weight(0.f)
{

}

bool UTestGoal::ReceiveIsApplicable_Implementation(const URpaiState* GivenState) const
{
   return true;
}

bool UTestGoal::ReceiveIsInDesiredState_Implementation(const URpaiState* GivenState) const
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

//Unordered planning distance
float UTestGoal::ReceiveGetDistanceToDesiredState_Implementation(const URpaiState* GivenState) const
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

float UTestGoal::ReceiveGetWeight_Implementation(const URpaiState* GivenState) const
{
   return Weight;
}

const FName UTestGoal::ConditionAKey = "TestA";
const FName UTestGoal::ConditionBKey = "TestB";
const FName UTestGoal::ConditionCKey = "TestC";