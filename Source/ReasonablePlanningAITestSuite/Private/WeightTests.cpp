// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "ReasonablePlanningAITestTypes.h"
#include "Composer/RpaiComposerWeightBase.h"
#include "Composer/Weights/RpaiWeight_Float.h"
#include "Composer/Weights/RpaiWeight_CurveFloat.h"
#include "Composer/Weights/RpaiWeight_ConstantFloat.h"

BEGIN_DEFINE_SPEC(ReasonablePlanningWeightConstantFloatSpec, "ReasonablePlanningAI.Weights.ConstantFloat", EAutomationTestFlags::ProductFilter | EAutomationTestFlags_ApplicationContextMask)
   URpaiWeight_ConstantFloat* ClassUnderTest;
   UTestPlanningState* GivenState;
END_DEFINE_SPEC(ReasonablePlanningWeightConstantFloatSpec)
void ReasonablePlanningWeightConstantFloatSpec::Define()
{
   BeforeEach([this]()
      {
         ClassUnderTest = NewObject<URpaiWeight_ConstantFloat>();
         GivenState = NewObject<UTestPlanningState>();
      });

   Describe("Weight from float property", [this]()
      {
         It("Should return the default float value", [this]()
            {
               float ExpectedValue = 0.f;
               TestEqual("ExecutionWeight", ClassUnderTest->ExecutionWeight(GivenState), ExpectedValue);
            });

         It("Should return the set float value", [this]()
            {
               ClassUnderTest->ConstantWeight = 456.78f;
               TestEqual("ExecutionWeight", ClassUnderTest->ExecutionWeight(GivenState), ClassUnderTest->ConstantWeight);
            });

      });

   AfterEach([this]()
      {
         ClassUnderTest->ConditionalBeginDestroy();
         GivenState->ConditionalBeginDestroy();
      });
}

BEGIN_DEFINE_SPEC(ReasonablePlanningWeightFloatSpec, "ReasonablePlanningAI.Weights.Float", EAutomationTestFlags::ProductFilter | EAutomationTestFlags_ApplicationContextMask)
   URpaiWeight_Float* ClassUnderTest;
   UTestPlanningState* GivenState;
END_DEFINE_SPEC(ReasonablePlanningWeightFloatSpec)
void ReasonablePlanningWeightFloatSpec::Define()
{
   BeforeEach([this]()
      {
         ClassUnderTest = NewObject<URpaiWeight_Float>();
         GivenState = NewObject<UTestPlanningState>();
      });

   Describe("Weight from float property", [this]()
      {
         It("Should return the defined float field", [this]()
            {
               float ExpectedValue = 0.f;
               TestTrue("GetFloat", GivenState->GetFloat(UTestPlanningState::NAME_TheFloatValue, ExpectedValue));
               ClassUnderTest->SetWeightStateKeyAndType(UTestPlanningState::NAME_TheFloatValue, EStatePropertyType::Float);
               TestEqual("ExecutionWeight", ClassUnderTest->ExecutionWeight(GivenState), ExpectedValue);
            });

         It("Should return an insignificant weight with invalid key name", [this]()
            {
               ClassUnderTest->SetWeightStateKeyAndType(NAME_None, EStatePropertyType::Float);
               TestEqual("ExecutionWeight", ClassUnderTest->ExecutionWeight(GivenState), 0.f);
            });

         It("Should return an insignificant weight with invalid expected type name", [this]()
            {
               ClassUnderTest->SetWeightStateKeyAndType(UTestPlanningState::NAME_TheFloatValue, EStatePropertyType::Int);
               TestEqual("ExecutionWeight", ClassUnderTest->ExecutionWeight(GivenState), 0.f);
            });
      });

   AfterEach([this]()
      {
         ClassUnderTest->ConditionalBeginDestroy();
         GivenState->ConditionalBeginDestroy();
      });
}

BEGIN_DEFINE_SPEC(ReasonablePlanningWeightCurveFloatSpec, "ReasonablePlanningAI.Weights.CurveFloat", EAutomationTestFlags::ProductFilter | EAutomationTestFlags_ApplicationContextMask)
   URpaiWeight_CurveFloat* ClassUnderTest;
   UTestPlanningState* GivenState;
   UCurveFloat* GivenCurve;
END_DEFINE_SPEC(ReasonablePlanningWeightCurveFloatSpec)
void ReasonablePlanningWeightCurveFloatSpec::Define()
{

   BeforeEach([this]()
      {
         ClassUnderTest = NewObject<URpaiWeight_CurveFloat>();
         GivenState = NewObject<UTestPlanningState>();
         GivenCurve = NewObject<UCurveFloat>();
         GivenCurve->CreateCurveFromCSVString("0,0\r\n1,2\r\n");
         ClassUnderTest->SetCurve(GivenCurve);
      });


   Describe("Weight from float property", [this]()
      {
         It("Should return the defined float from curve using the float state property as input", [this]()
            {
               GivenState->SetFloat(UTestPlanningState::NAME_TheFloatValue, 0.5f);
               ClassUnderTest->SetWeightStateKeyAndType(UTestPlanningState::NAME_TheFloatValue, EStatePropertyType::Float);
               TestEqual("ExecutionWeight", ClassUnderTest->ExecutionWeight(GivenState), 1.0f);
            });

         It("Should return an insignificant weight with invalid key name", [this]()
            {
               ClassUnderTest->SetWeightStateKeyAndType(NAME_None, EStatePropertyType::Float);
               TestEqual("ExecutionWeight", ClassUnderTest->ExecutionWeight(GivenState), 0.f);
            });

         It("Should return an insignificant weight with invalid expected type name", [this]()
            {
               ClassUnderTest->SetWeightStateKeyAndType(UTestPlanningState::NAME_TheFloatValue, EStatePropertyType::Int);
               TestEqual("ExecutionWeight", ClassUnderTest->ExecutionWeight(GivenState), 0.f);
            });
      });

   AfterEach([this]()
      {
         ClassUnderTest->ConditionalBeginDestroy();
         GivenState->ConditionalBeginDestroy();
         GivenCurve->ConditionalBeginDestroy();
      });
}
