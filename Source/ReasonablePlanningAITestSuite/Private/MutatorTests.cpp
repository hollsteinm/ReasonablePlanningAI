// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "ReasonablePlanningAITestTypes.h"
#include "States/RpaiState_Map.h"
#include "Composer/Mutators/RpaiStateMutator_AddFloat.h"
#include "Composer/Mutators/RpaiStateMutator_AddInteger.h"
#include "Composer/Mutators/RpaiStateMutator_MultiplyFloat.h"
#include "Composer/Mutators/RpaiStateMutator_MultiplyInteger.h"
#include "Composer/Mutators/RpaiStateMutator_SetValueBool.h"
#include "Composer/Mutators/RpaiStateMutator_SetValueClass.h"
#include "Composer/Mutators/RpaiStateMutator_SetValueFloat.h"
#include "Composer/Mutators/RpaiStateMutator_SetValueInteger.h"
#include "Composer/Mutators/RpaiStateMutator_SetValueName.h"
#include "Composer/Mutators/RpaiStateMutator_SetValueRotator.h"
#include "Composer/Mutators/RpaiStateMutator_SetValueString.h"
#include "Composer/Mutators/RpaiStateMutator_SetValueVector.h"
#include "Composer/Mutators/RpaiStateMutator_CopyState.h"

BEGIN_DEFINE_SPEC(ReasonablePlanningMutatorAddFloatSpec, "ReasonablePlanningAI.Mutators.AddFloat", EAutomationTestFlags::ProductFilter | EAutomationTestFlags_ApplicationContextMask)
   URpaiStateMutator_AddFloat* ClassUnderTest;
   UTestPlanningState* GivenState;
END_DEFINE_SPEC(ReasonablePlanningMutatorAddFloatSpec)
void ReasonablePlanningMutatorAddFloatSpec::Define()
{
   BeforeEach([this]()
      {
         ClassUnderTest = NewObject<URpaiStateMutator_AddFloat>();
         GivenState = NewObject<UTestPlanningState>();

         ClassUnderTest->SetMutatedStateValue(UTestPlanningState::NAME_TheFloatValue, EStatePropertyType::Float);
      });

   Describe("Add Float to state", [this]()
      {
         It("Should add the value", [this]()
            {
               GivenState->SetValueOfType(UTestPlanningState::NAME_TheFloatValue, 10.f);
               ClassUnderTest->SetFloatValueToAdd(10.f);
               
               ClassUnderTest->Mutate(GivenState);

               float ActualValue = 0.f;
               TestTrue("Getting Value", GivenState->GetValueOfType(UTestPlanningState::NAME_TheFloatValue, ActualValue));
               TestEqual("State Mutated", ActualValue, 20.f);
            });

         It("Should subtract when given a negative value", [this]()
            {
               GivenState->SetValueOfType(UTestPlanningState::NAME_TheFloatValue, 100.f);
               ClassUnderTest->SetFloatValueToAdd(-10.f);

               ClassUnderTest->Mutate(GivenState);

               float ActualValue = 0.f;
               TestTrue("Getting Value", GivenState->GetValueOfType(UTestPlanningState::NAME_TheFloatValue, ActualValue));
               TestEqual("State Mutated", ActualValue, 90.f);
            });
      });

   AfterEach([this]()
      {
         ClassUnderTest->ConditionalBeginDestroy();
         GivenState->ConditionalBeginDestroy();
      });
}

BEGIN_DEFINE_SPEC(ReasonablePlanningMutatorAddIntegerSpec, "ReasonablePlanningAI.Mutators.AddInteger", EAutomationTestFlags::ProductFilter | EAutomationTestFlags_ApplicationContextMask)
   URpaiStateMutator_AddInteger* ClassUnderTest;
   UTestPlanningState* GivenState;
END_DEFINE_SPEC(ReasonablePlanningMutatorAddIntegerSpec)
void ReasonablePlanningMutatorAddIntegerSpec::Define()
{
   BeforeEach([this]()
      {
         ClassUnderTest = NewObject<URpaiStateMutator_AddInteger>();
         GivenState = NewObject<UTestPlanningState>();

         ClassUnderTest->SetMutatedStateValue(UTestPlanningState::NAME_TheIntValue, EStatePropertyType::Int);
      });

   Describe("Add Integer to state", [this]()
      {
         It("Should add the value", [this]()
            {
               GivenState->SetValueOfType(UTestPlanningState::NAME_TheIntValue, 10);
               ClassUnderTest->SetIntegerValueToAdd(10);

               ClassUnderTest->Mutate(GivenState);

               int32 ActualValue = 0;
               TestTrue("Getting Value", GivenState->GetValueOfType(UTestPlanningState::NAME_TheIntValue, ActualValue));
               TestEqual("State Mutated", ActualValue, 20);
            });

         It("Should subtract when given a negative value", [this]()
            {
               GivenState->SetValueOfType(UTestPlanningState::NAME_TheIntValue, 100);
               ClassUnderTest->SetIntegerValueToAdd(-10);

               ClassUnderTest->Mutate(GivenState);

               int32 ActualValue = 0;
               TestTrue("Getting Value", GivenState->GetValueOfType(UTestPlanningState::NAME_TheIntValue, ActualValue));
               TestEqual("State Mutated", ActualValue, 90);
            });
      });

   AfterEach([this]()
      {
         ClassUnderTest->ConditionalBeginDestroy();
         GivenState->ConditionalBeginDestroy();
      });
}

BEGIN_DEFINE_SPEC(ReasonablePlanningMutatorMultiplyFloatSpec, "ReasonablePlanningAI.Mutators.MultiplyFloat", EAutomationTestFlags::ProductFilter | EAutomationTestFlags_ApplicationContextMask)
   URpaiStateMutator_MultiplyFloat* ClassUnderTest;
   UTestPlanningState* GivenState;
END_DEFINE_SPEC(ReasonablePlanningMutatorMultiplyFloatSpec)
void ReasonablePlanningMutatorMultiplyFloatSpec::Define()
{
   BeforeEach([this]()
      {
         ClassUnderTest = NewObject<URpaiStateMutator_MultiplyFloat>();
         GivenState = NewObject<UTestPlanningState>();

         ClassUnderTest->SetMutatedStateValue(UTestPlanningState::NAME_TheFloatValue, EStatePropertyType::Float);
      });

   Describe("Multiply Float to state", [this]()
      {
         It("Should multiply the value", [this]()
            {
               GivenState->SetValueOfType(UTestPlanningState::NAME_TheFloatValue, 10.f);
               ClassUnderTest->SetFloatMultiplierValue(10.f);

               ClassUnderTest->Mutate(GivenState);

               float ActualValue = 0.f;
               TestTrue("Getting Value", GivenState->GetValueOfType(UTestPlanningState::NAME_TheFloatValue, ActualValue));
               TestEqual("State Mutated", ActualValue, 100.f);
            });

         It("Should multiply when given a decimal", [this]()
            {
               GivenState->SetValueOfType(UTestPlanningState::NAME_TheFloatValue, 100.f);
               ClassUnderTest->SetFloatMultiplierValue(0.1f);

               ClassUnderTest->Mutate(GivenState);

               float ActualValue = 0.f;
               TestTrue("Getting Value", GivenState->GetValueOfType(UTestPlanningState::NAME_TheFloatValue, ActualValue));
               TestEqual("State Mutated", ActualValue, 10.f);
            });

         It("Should multiply when given a decimal with state as integer", [this]()
            {
               ClassUnderTest->SetMutatedStateValue(UTestPlanningState::NAME_TheIntValue, EStatePropertyType::Int);
               GivenState->SetValueOfType(UTestPlanningState::NAME_TheIntValue, 60);
               ClassUnderTest->SetFloatMultiplierValue(0.5f);

               ClassUnderTest->Mutate(GivenState);

               int32 ActualValue = 0;
               TestTrue("Getting Value", GivenState->GetValueOfType(UTestPlanningState::NAME_TheIntValue, ActualValue));
               TestEqual("State Mutated", ActualValue, 30);
            });
      });

   AfterEach([this]()
      {
         ClassUnderTest->ConditionalBeginDestroy();
         GivenState->ConditionalBeginDestroy();
      });
}

BEGIN_DEFINE_SPEC(ReasonablePlanningMutatorMultiplyIntegerSpec, "ReasonablePlanningAI.Mutators.MulitplyInteger", EAutomationTestFlags::ProductFilter | EAutomationTestFlags_ApplicationContextMask)
   URpaiStateMutator_MultiplyInteger* ClassUnderTest;
   UTestPlanningState* GivenState;
END_DEFINE_SPEC(ReasonablePlanningMutatorMultiplyIntegerSpec)
void ReasonablePlanningMutatorMultiplyIntegerSpec::Define()
{
   BeforeEach([this]()
      {
         ClassUnderTest = NewObject<URpaiStateMutator_MultiplyInteger>();
         GivenState = NewObject<UTestPlanningState>();

         ClassUnderTest->SetMutatedStateValue(UTestPlanningState::NAME_TheIntValue, EStatePropertyType::Int);
      });

   Describe("Add Integer to state", [this]()
      {
         It("Should add the value", [this]()
            {
               GivenState->SetValueOfType(UTestPlanningState::NAME_TheIntValue, 10);
               ClassUnderTest->SetIntegerMultiplierValue(10);

               ClassUnderTest->Mutate(GivenState);

               int32 ActualValue = 0;
               TestTrue("Getting Value", GivenState->GetValueOfType(UTestPlanningState::NAME_TheIntValue, ActualValue));
               TestEqual("State Mutated", ActualValue, 100);
            });

      });

   AfterEach([this]()
      {
         ClassUnderTest->ConditionalBeginDestroy();
         GivenState->ConditionalBeginDestroy();
      });
}

BEGIN_DEFINE_SPEC(ReasonablePlanningMutatorSetBoolSpec, "ReasonablePlanningAI.Mutators.SetBool", EAutomationTestFlags::ProductFilter | EAutomationTestFlags_ApplicationContextMask)
   URpaiStateMutator_SetValueBool* ClassUnderTest;
   UTestPlanningState* GivenState;
END_DEFINE_SPEC(ReasonablePlanningMutatorSetBoolSpec)
void ReasonablePlanningMutatorSetBoolSpec::Define()
{
   BeforeEach([this]()
      {
         ClassUnderTest = NewObject<URpaiStateMutator_SetValueBool>();
         GivenState = NewObject<UTestPlanningState>();

         ClassUnderTest->SetMutatedStateValue(UTestPlanningState::NAME_TheBoolValue, EStatePropertyType::Bool);
      });

   Describe("Set bool on state", [this]()
      {
         It("Should set the value", [this]()
            {
               GivenState->SetValueOfType(UTestPlanningState::NAME_TheBoolValue, false);
               ClassUnderTest->SetValueToSet(true);

               ClassUnderTest->Mutate(GivenState);

               bool ActualValue = false;
               TestTrue("Getting Value", GivenState->GetValueOfType(UTestPlanningState::NAME_TheBoolValue, ActualValue));
               TestEqual("State Mutated", ActualValue, true);
            });

      });

   AfterEach([this]()
      {
         ClassUnderTest->ConditionalBeginDestroy();
         GivenState->ConditionalBeginDestroy();
      });
}

BEGIN_DEFINE_SPEC(ReasonablePlanningMutatorSetClassSpec, "ReasonablePlanningAI.Mutators.SetClass", EAutomationTestFlags::ProductFilter | EAutomationTestFlags_ApplicationContextMask)
   URpaiStateMutator_SetValueClass* ClassUnderTest;
   UTestPlanningState* GivenState;
END_DEFINE_SPEC(ReasonablePlanningMutatorSetClassSpec)
void ReasonablePlanningMutatorSetClassSpec::Define()
{
   BeforeEach([this]()
      {
         ClassUnderTest = NewObject<URpaiStateMutator_SetValueClass>();
         GivenState = NewObject<UTestPlanningState>();

         ClassUnderTest->SetMutatedStateValue(UTestPlanningState::NAME_TheClassValue, EStatePropertyType::Class);
      });

   Describe("Set class on state", [this]()
      {
         It("Should set the value", [this]()
            {
               GivenState->SetValueOfType(UTestPlanningState::NAME_TheClassValue, UTestPlanningState::StaticClass());
               ClassUnderTest->SetValueToSet(URpaiStateMutator_SetValueClass::StaticClass());

               ClassUnderTest->Mutate(GivenState);

               UClass* ActualValue = nullptr;
               TestTrue("Getting Value", GivenState->GetValueOfType(UTestPlanningState::NAME_TheClassValue, ActualValue));
               TestEqual("State Mutated", ActualValue, URpaiStateMutator_SetValueClass::StaticClass());
            });

      });

   AfterEach([this]()
      {
         ClassUnderTest->ConditionalBeginDestroy();
         GivenState->ConditionalBeginDestroy();
      });
}

BEGIN_DEFINE_SPEC(ReasonablePlanningMutatorSetFloatSpec, "ReasonablePlanningAI.Mutators.SetFloat", EAutomationTestFlags::ProductFilter | EAutomationTestFlags_ApplicationContextMask)
   URpaiStateMutator_SetValueFloat* ClassUnderTest;
   UTestPlanningState* GivenState;
END_DEFINE_SPEC(ReasonablePlanningMutatorSetFloatSpec)
void ReasonablePlanningMutatorSetFloatSpec::Define()
{
   BeforeEach([this]()
      {
         ClassUnderTest = NewObject<URpaiStateMutator_SetValueFloat>();
         GivenState = NewObject<UTestPlanningState>();

         ClassUnderTest->SetMutatedStateValue(UTestPlanningState::NAME_TheFloatValue, EStatePropertyType::Float);
      });

   Describe("Set float on state", [this]()
      {
         It("Should set the value", [this]()
            {
               GivenState->SetValueOfType(UTestPlanningState::NAME_TheFloatValue, 34.0f);
               ClassUnderTest->SetValueToSet(-800.f);

               ClassUnderTest->Mutate(GivenState);

               float ActualValue = 0.f;
               TestTrue("Getting Value", GivenState->GetValueOfType(UTestPlanningState::NAME_TheFloatValue, ActualValue));
               TestEqual("State Mutated", ActualValue, -800.f);
            });

      });

   AfterEach([this]()
      {
         ClassUnderTest->ConditionalBeginDestroy();
         GivenState->ConditionalBeginDestroy();
      });
}

BEGIN_DEFINE_SPEC(ReasonablePlanningMutatorSetIntegerSpec, "ReasonablePlanningAI.Mutators.SetInteger", EAutomationTestFlags::ProductFilter | EAutomationTestFlags_ApplicationContextMask)
   URpaiStateMutator_SetValueInteger* ClassUnderTest;
   UTestPlanningState* GivenState;
END_DEFINE_SPEC(ReasonablePlanningMutatorSetIntegerSpec)
void ReasonablePlanningMutatorSetIntegerSpec::Define()
{
   BeforeEach([this]()
      {
         ClassUnderTest = NewObject<URpaiStateMutator_SetValueInteger>();
         GivenState = NewObject<UTestPlanningState>();

         ClassUnderTest->SetMutatedStateValue(UTestPlanningState::NAME_TheIntValue, EStatePropertyType::Int);
      });

   Describe("Set integer on state", [this]()
      {
         It("Should set the value", [this]()
            {
               GivenState->SetValueOfType(UTestPlanningState::NAME_TheIntValue, -9999);
               ClassUnderTest->SetValueToSet(4545);

               ClassUnderTest->Mutate(GivenState);

               int32 ActualValue = 0;
               TestTrue("Getting Value", GivenState->GetValueOfType(UTestPlanningState::NAME_TheIntValue, ActualValue));
               TestEqual("State Mutated", ActualValue, 4545);
            });

      });

   AfterEach([this]()
      {
         ClassUnderTest->ConditionalBeginDestroy();
         GivenState->ConditionalBeginDestroy();
      });
}

BEGIN_DEFINE_SPEC(ReasonablePlanningMutatorSetNameSpec, "ReasonablePlanningAI.Mutators.SetName", EAutomationTestFlags::ProductFilter | EAutomationTestFlags_ApplicationContextMask)
   URpaiStateMutator_SetValueName* ClassUnderTest;
   UTestPlanningState* GivenState;
END_DEFINE_SPEC(ReasonablePlanningMutatorSetNameSpec)
void ReasonablePlanningMutatorSetNameSpec::Define()
{
   BeforeEach([this]()
      {
         ClassUnderTest = NewObject<URpaiStateMutator_SetValueName>();
         GivenState = NewObject<UTestPlanningState>();

         ClassUnderTest->SetMutatedStateValue(UTestPlanningState::NAME_TheNameValue, EStatePropertyType::Name);
      });

   Describe("Set name on state", [this]()
      {
         It("Should set the value", [this]()
            {
               GivenState->SetValueOfType(UTestPlanningState::NAME_TheIntValue, FName(NAME_None));
               ClassUnderTest->SetValueToSet(NAME_Actor);

               ClassUnderTest->Mutate(GivenState);

               FName ActualValue = NAME_None;
               TestTrue("Getting Value", GivenState->GetValueOfType(UTestPlanningState::NAME_TheNameValue, ActualValue));
               TestEqual("State Mutated", ActualValue, FName(NAME_Actor));
            });

      });

   AfterEach([this]()
      {
         ClassUnderTest->ConditionalBeginDestroy();
         GivenState->ConditionalBeginDestroy();
      });
}

BEGIN_DEFINE_SPEC(ReasonablePlanningMutatorSetRotatorSpec, "ReasonablePlanningAI.Mutators.SetRotator", EAutomationTestFlags::ProductFilter | EAutomationTestFlags_ApplicationContextMask)
   URpaiStateMutator_SetValueRotator* ClassUnderTest;
   UTestPlanningState* GivenState;
END_DEFINE_SPEC(ReasonablePlanningMutatorSetRotatorSpec)
void ReasonablePlanningMutatorSetRotatorSpec::Define()
{
   BeforeEach([this]()
      {
         ClassUnderTest = NewObject<URpaiStateMutator_SetValueRotator>();
         GivenState = NewObject<UTestPlanningState>();

         ClassUnderTest->SetMutatedStateValue(UTestPlanningState::NAME_TheRotatorValue, EStatePropertyType::Rotator);
      });

   Describe("Set rotator on state", [this]()
      {
         It("Should set the value", [this]()
            {
               FRotator ExpectedValue(40.f, 56.f, 80.f);
               GivenState->SetValueOfType(UTestPlanningState::NAME_TheRotatorValue, FRotator(20.f, 3.f, 100.f));
               ClassUnderTest->SetValueToSet(ExpectedValue);

               ClassUnderTest->Mutate(GivenState);

               FRotator ActualValue;
               TestTrue("Getting Value", GivenState->GetValueOfType(UTestPlanningState::NAME_TheRotatorValue, ActualValue));
               TestEqual("State Mutated", ActualValue, ExpectedValue);
            });

      });

   AfterEach([this]()
      {
         ClassUnderTest->ConditionalBeginDestroy();
         GivenState->ConditionalBeginDestroy();
      });
}

BEGIN_DEFINE_SPEC(ReasonablePlanningMutatorSetStringSpec, "ReasonablePlanningAI.Mutators.SetString", EAutomationTestFlags::ProductFilter | EAutomationTestFlags_ApplicationContextMask)
   URpaiStateMutator_SetValueString* ClassUnderTest;
   UTestPlanningState* GivenState;
END_DEFINE_SPEC(ReasonablePlanningMutatorSetStringSpec)
void ReasonablePlanningMutatorSetStringSpec::Define()
{
   BeforeEach([this]()
      {
         ClassUnderTest = NewObject<URpaiStateMutator_SetValueString>();
         GivenState = NewObject<UTestPlanningState>();

         ClassUnderTest->SetMutatedStateValue(UTestPlanningState::NAME_TheStringValue, EStatePropertyType::String);
      });

   Describe("Set string on state", [this]()
      {
         It("Should set the value", [this]()
            {
               FString ExpectedValue("Test String");
               GivenState->SetValueOfType(UTestPlanningState::NAME_TheStringValue, FString("Not Expected"));
               ClassUnderTest->SetValueToSet(ExpectedValue);

               ClassUnderTest->Mutate(GivenState);

               FString ActualValue;
               TestTrue("Getting Value", GivenState->GetValueOfType(UTestPlanningState::NAME_TheStringValue, ActualValue));
               TestEqual("State Mutated", ActualValue, ExpectedValue);
            });

      });

   AfterEach([this]()
      {
         ClassUnderTest->ConditionalBeginDestroy();
         GivenState->ConditionalBeginDestroy();
      });
}

BEGIN_DEFINE_SPEC(ReasonablePlanningMutatorSetVectorSpec, "ReasonablePlanningAI.Mutators.SetVector", EAutomationTestFlags::ProductFilter | EAutomationTestFlags_ApplicationContextMask)
   URpaiStateMutator_SetValueVector* ClassUnderTest;
   UTestPlanningState* GivenState;
END_DEFINE_SPEC(ReasonablePlanningMutatorSetVectorSpec)
void ReasonablePlanningMutatorSetVectorSpec::Define()
{
   BeforeEach([this]()
      {
         ClassUnderTest = NewObject<URpaiStateMutator_SetValueVector>();
         GivenState = NewObject<UTestPlanningState>();

         ClassUnderTest->SetMutatedStateValue(UTestPlanningState::NAME_TheVectorValue, EStatePropertyType::Vector);
      });

   Describe("Set vector on state", [this]()
      {
         It("Should set the value", [this]()
            {
               FVector ExpectedValue(1.4f, 2.4f, 2.4f);
               GivenState->SetValueOfType(UTestPlanningState::NAME_TheVectorValue, FVector(100.f, 400.f, 300.f));
               ClassUnderTest->SetValueToSet(ExpectedValue);

               ClassUnderTest->Mutate(GivenState);

               FVector ActualValue;
               TestTrue("Getting Value", GivenState->GetValueOfType(UTestPlanningState::NAME_TheVectorValue, ActualValue));
               TestEqual("State Mutated", ActualValue, ExpectedValue);
            });

      });

   AfterEach([this]()
      {
         ClassUnderTest->ConditionalBeginDestroy();
         GivenState->ConditionalBeginDestroy();
      });
}

BEGIN_DEFINE_SPEC(ReasonablePlanningMutatorCopyStateSpec, "ReasonablePlanningAI.Mutators.CopyState", EAutomationTestFlags::ProductFilter | EAutomationTestFlags_ApplicationContextMask)
   URpaiStateMutator_CopyState* ClassUnderTest;
   URpaiState* GivenState;
END_DEFINE_SPEC(ReasonablePlanningMutatorCopyStateSpec)
void ReasonablePlanningMutatorCopyStateSpec::Define()
{
   BeforeEach([this]()
      {
         ClassUnderTest = NewObject<URpaiStateMutator_CopyState>();
         URpaiState_Map* TestMap = NewObject<URpaiState_Map>();
         GivenState = TestMap;

         TestMap->SetAsDynamic(true);
         TestMap->SetValueOfType("FA", 1.f);
         TestMap->SetValueOfType("FB", 0.f);
         TestMap->SetValueOfType("BA", true);
         TestMap->SetValueOfType("BB", false);
         TestMap->SetValueOfType("IA", 23);
         TestMap->SetValueOfType("IB", 0);
         TestMap->SetAsDynamic(false);
      });

   Describe("Copying State", [this]()
      {
         It("Should copy the float values", [this]()
            {
               ClassUnderTest->SetMutatedStateValue("FB", EStatePropertyType::Float);
               ClassUnderTest->SetCopiedFromStateValue("FA", EStatePropertyType::Float);

               ClassUnderTest->Mutate(GivenState);

               float A = 0.f;
               float B = 0.f;

               GivenState->GetValueOfType("FA", A);
               GivenState->GetValueOfType("FB", B);

               TestEqual("Initial value unchanged", A, 1.f);
               TestEqual("Target value changed", B, A);
            });

         It("should copy the boolean values", [this]()
            {
               ClassUnderTest->SetMutatedStateValue("BB", EStatePropertyType::Bool);
               ClassUnderTest->SetCopiedFromStateValue("BA", EStatePropertyType::Bool);

               ClassUnderTest->Mutate(GivenState);

               bool bA = false;
               bool bB = false;

               GivenState->GetValueOfType("BA", bA);
               GivenState->GetValueOfType("BB", bB);

               TestEqual("Initial value unchanged", bA, true);
               TestEqual("Target value changed", bB, bA);
            });

         It("should copy the integer values", [this]()
            {
               ClassUnderTest->SetMutatedStateValue("IB", EStatePropertyType::Int);
               ClassUnderTest->SetCopiedFromStateValue("IA", EStatePropertyType::Int);

               ClassUnderTest->Mutate(GivenState);

               int32 A = 0;
               int32 B = 0;

               GivenState->GetValueOfType("IA", A);
               GivenState->GetValueOfType("IB", B);

               TestEqual("Initial value unchanged", A, 23);
               TestEqual("Target value changed", B, A);
            });

      });

   AfterEach([this]()
      {
         ClassUnderTest->ConditionalBeginDestroy();
         GivenState->ConditionalBeginDestroy();
      });
}