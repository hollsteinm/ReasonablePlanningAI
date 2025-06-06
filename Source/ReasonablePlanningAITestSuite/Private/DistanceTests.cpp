// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "ReasonablePlanningAITestTypes.h"
#include "Composer/RpaiComposerDistance.h"
#include "Composer/Distances/RpaiDistance_State.h"
#include "Composer/Distances/RpaiDistance_Bool.h"
#include "Composer/Distances/RpaiDistance_Float.h"
#include "Composer/Distances/RpaiDistance_Integer.h"
#include "Composer/Distances/RpaiDistance_Rotator.h"
#include "Composer/Distances/RpaiDistance_Vector.h"
#include "Composer/Distances/RpaiDistance_CurveFloat.h"
#include "States/RpaiState_Map.h"

BEGIN_DEFINE_SPEC(ReasonablePlanningDistanceBoolSpec, "ReasonablePlanningAI.Distance.Bool", EAutomationTestFlags::ProductFilter | EAutomationTestFlags_ApplicationContextMask)
    URpaiDistance_Bool* ClassUnderTest;
    URpaiState* GivenState;
END_DEFINE_SPEC(ReasonablePlanningDistanceBoolSpec)
void ReasonablePlanningDistanceBoolSpec::Define()
{
    Describe("Calculating Distance for bool type", [this]()
        {
            BeforeEach([this]()
                {
                    auto MapState = NewObject<URpaiState_Map>();
                    MapState->SetAsDynamic(true);
                    
                    ClassUnderTest = NewObject<URpaiDistance_Bool>();
                    GivenState = MapState;
                });
        
            It("Should return a 0 or 1 as the difference between two bools", [this]()
               {
                    ClassUnderTest->SetLHS("LHS", EStatePropertyType::Bool);

                    GivenState->SetBool("LHS", true);
               ClassUnderTest->SetRHS(true);
                    TestEqual("URpaiDistance_Bool::CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), 0.f);

                    GivenState->SetBool("LHS", true);
                    ClassUnderTest->SetRHS(false);
                    TestEqual("URpaiDistance_Bool:::CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), 1.f);

                    GivenState->SetBool("LHS", false);
                    ClassUnderTest->SetRHS(false);
                    TestEqual("URpaiDistance_Bool:::CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), 0.f);

                    GivenState->SetBool("LHS", false);
                    ClassUnderTest->SetRHS(true);
                    TestEqual("URpaiDistance_Bool:::CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), 1.f);
                });
        
            It("Should return max value if the state value is a class", [this]()
                {
                    ClassUnderTest->SetLHS("LHS", EStatePropertyType::Class);
               TestEqual("URpaiDistance_Bool:::CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), TNumericLimits<float>::Max());
               
                });

         It("Should return max value if the state value is a enum", [this]()
            {
               ClassUnderTest->SetLHS("LHS", EStatePropertyType::Enum);
               TestEqual("URpaiDistance_Bool:::CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), TNumericLimits<float>::Max());

            });

         It("Should return max value if the state value is a float", [this]()
            {
               ClassUnderTest->SetLHS("LHS", EStatePropertyType::Float);
               TestEqual("URpaiDistance_Bool:::CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), TNumericLimits<float>::Max());

            });

         It("Should return max value if the state value is a int", [this]()
            {
               ClassUnderTest->SetLHS("LHS", EStatePropertyType::Int);
               TestEqual("URpaiDistance_Bool:::CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), TNumericLimits<float>::Max());

            });

         It("Should return max value if the state value is a name", [this]()
            {
               ClassUnderTest->SetLHS("LHS", EStatePropertyType::Name);
               TestEqual("URpaiDistance_Bool:::CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), TNumericLimits<float>::Max());

            });

         It("Should return max value if the state value is a object", [this]()
            {
               ClassUnderTest->SetLHS("LHS", EStatePropertyType::Object);
               TestEqual("URpaiDistance_Bool:::CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), TNumericLimits<float>::Max());

            });

         It("Should return max value if the state value is a rotator", [this]()
            {
               ClassUnderTest->SetLHS("LHS", EStatePropertyType::Rotator);
               TestEqual("URpaiDistance_Bool:::CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), TNumericLimits<float>::Max());

            });

         It("Should return max value if the state value is a string", [this]()
            {
               ClassUnderTest->SetLHS("LHS", EStatePropertyType::String);
               TestEqual("URpaiDistance_Bool:::CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), TNumericLimits<float>::Max());

            });

         It("Should return max value if the state value is a vector", [this]()
            {
               ClassUnderTest->SetLHS("LHS", EStatePropertyType::Vector);
               TestEqual("URpaiDistance_Bool:::CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), TNumericLimits<float>::Max());

            });
        
            AfterEach([this]()
                {
                    ClassUnderTest->ConditionalBeginDestroy();
                    GivenState->ConditionalBeginDestroy();
                });
        });
}

BEGIN_DEFINE_SPEC(ReasonablePlanningDistanceFloatSpec, "ReasonablePlanningAI.Distance.Float", EAutomationTestFlags::ProductFilter | EAutomationTestFlags_ApplicationContextMask)
   URpaiDistance_Float* ClassUnderTest;
   URpaiState* GivenState;
END_DEFINE_SPEC(ReasonablePlanningDistanceFloatSpec)
void ReasonablePlanningDistanceFloatSpec::Define()
{
   Describe("Calculating Distance for float type as an absolute value", [this]()
      {
         BeforeEach([this]()
            {
               auto MapState = NewObject<URpaiState_Map>();
               MapState->SetAsDynamic(true);

               ClassUnderTest = NewObject<URpaiDistance_Float>();
               GivenState = MapState;
            });

         It("Should return the difference from LHS to RHS", [this]()
            {
               ClassUnderTest->SetLHS("LHS", EStatePropertyType::Float);
               GivenState->SetFloat("LHS", 50.0f);
               ClassUnderTest->SetRHS(100.0f);
               TestEqual("URpaiDistance_Float::CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), 50.f);
            });

         It("Should support integer differences as well for LHS", [this]()
            {
               ClassUnderTest->SetLHS("LHS", EStatePropertyType::Int);
               GivenState->SetInt("LHS", 200);
               ClassUnderTest->SetRHS(54.6f);
               TestEqual("URpaiDistance_Float::CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), 146.f);
            });

         It("Should return max value if the state value is a class", [this]()
            {
               ClassUnderTest->SetLHS("LHS", EStatePropertyType::Class);
               TestEqual("URpaiDistance_Bool:::CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), TNumericLimits<float>::Max());
            });

         It("Should return max value if the state value is a enum", [this]()
            {
               ClassUnderTest->SetLHS("LHS", EStatePropertyType::Enum);
               TestEqual("URpaiDistance_Bool:::CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), TNumericLimits<float>::Max());
            });

         It("Should return max value if the state value is a bool", [this]()
            {
               ClassUnderTest->SetLHS("LHS", EStatePropertyType::Bool);
               TestEqual("URpaiDistance_Bool:::CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), TNumericLimits<float>::Max());
            });

         It("Should return max value if the state value is a name", [this]()
            {
               ClassUnderTest->SetLHS("LHS", EStatePropertyType::Name);
               TestEqual("URpaiDistance_Bool:::CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), TNumericLimits<float>::Max());
            });

         It("Should return max value if the state value is a object", [this]()
            {
               ClassUnderTest->SetLHS("LHS", EStatePropertyType::Object);
               TestEqual("URpaiDistance_Bool:::CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), TNumericLimits<float>::Max());
            });

         It("Should return max value if the state value is a rotator", [this]()
            {
               ClassUnderTest->SetLHS("LHS", EStatePropertyType::Rotator);
               TestEqual("URpaiDistance_Bool:::CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), TNumericLimits<float>::Max());
            });

         It("Should return max value if the state value is a string", [this]()
            {
               ClassUnderTest->SetLHS("LHS", EStatePropertyType::String);
               TestEqual("URpaiDistance_Bool:::CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), TNumericLimits<float>::Max());
            });

         It("Should return max value if the state value is a vector", [this]()
            {
               ClassUnderTest->SetLHS("LHS", EStatePropertyType::Vector);
               TestEqual("URpaiDistance_Bool:::CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), TNumericLimits<float>::Max());
            });

         AfterEach([this]()
            {
               ClassUnderTest->ConditionalBeginDestroy();
               GivenState->ConditionalBeginDestroy();
            });
      });
}

BEGIN_DEFINE_SPEC(ReasonablePlanningDistanceIntegerSpec, "ReasonablePlanningAI.Distance.Integer", EAutomationTestFlags::ProductFilter | EAutomationTestFlags_ApplicationContextMask)
   URpaiDistance_Integer* ClassUnderTest;
   URpaiState* GivenState;
END_DEFINE_SPEC(ReasonablePlanningDistanceIntegerSpec)
void ReasonablePlanningDistanceIntegerSpec::Define()
{
   Describe("Calculating Distance for float type as an absolute value", [this]()
      {
         BeforeEach([this]()
            {
               auto MapState = NewObject<URpaiState_Map>();
               MapState->SetAsDynamic(true);

               ClassUnderTest = NewObject<URpaiDistance_Integer>();
               GivenState = MapState;
            });

         It("Should return the difference from LHS to RHS", [this]()
            {
               ClassUnderTest->SetLHS("LHS", EStatePropertyType::Int);
               GivenState->SetInt("LHS", 25);
               ClassUnderTest->SetRHS(75);
               TestEqual("URpaiDistance_Float::CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), 50.f);

               GivenState->SetInt("LHS", 4000);
               ClassUnderTest->SetRHS(5);
               TestEqual("URpaiDistance_Float::CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), 3995.f);
            });

         It("Should support float differences as well for LHS", [this]()
            {
               ClassUnderTest->SetLHS("LHS", EStatePropertyType::Float);
               GivenState->SetFloat("LHS", 200.5f);
               ClassUnderTest->SetRHS(50);
               TestEqual("URpaiDistance_Float::CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), 150.5f);
            });

         It("Should return max value if the state value is a class", [this]()
            {
               ClassUnderTest->SetLHS("LHS", EStatePropertyType::Class);
               TestEqual("URpaiDistance_Bool:::CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), TNumericLimits<float>::Max());
            });

         It("Should return max value if the state value is a enum", [this]()
            {
               ClassUnderTest->SetLHS("LHS", EStatePropertyType::Enum);
               TestEqual("URpaiDistance_Bool:::CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), TNumericLimits<float>::Max());
            });

         It("Should return max value if the state value is a bool", [this]()
            {
               ClassUnderTest->SetLHS("LHS", EStatePropertyType::Bool);
               TestEqual("URpaiDistance_Bool:::CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), TNumericLimits<float>::Max());
            });

         It("Should return max value if the state value is a name", [this]()
            {
               ClassUnderTest->SetLHS("LHS", EStatePropertyType::Name);
               TestEqual("URpaiDistance_Bool:::CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), TNumericLimits<float>::Max());
            });

         It("Should return max value if the state value is a object", [this]()
            {
               ClassUnderTest->SetLHS("LHS", EStatePropertyType::Object);
               TestEqual("URpaiDistance_Bool:::CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), TNumericLimits<float>::Max());

            });

         It("Should return max value if the state value is a rotator", [this]()
            {
               ClassUnderTest->SetLHS("LHS", EStatePropertyType::Rotator);
               TestEqual("URpaiDistance_Bool:::CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), TNumericLimits<float>::Max());
            });

         It("Should return max value if the state value is a string", [this]()
            {
               ClassUnderTest->SetLHS("LHS", EStatePropertyType::String);
               TestEqual("URpaiDistance_Bool:::CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), TNumericLimits<float>::Max());
            });

         It("Should return max value if the state value is a vector", [this]()
            {
               ClassUnderTest->SetLHS("LHS", EStatePropertyType::Vector);
               TestEqual("URpaiDistance_Bool:::CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), TNumericLimits<float>::Max());
            });

         AfterEach([this]()
            {
               ClassUnderTest->ConditionalBeginDestroy();
               GivenState->ConditionalBeginDestroy();
            });
      });
}

BEGIN_DEFINE_SPEC(ReasonablePlanningDistanceRotatorSpec, "ReasonablePlanningAI.Distance.Rotator", EAutomationTestFlags::ProductFilter | EAutomationTestFlags_ApplicationContextMask)
   URpaiDistance_Rotator* ClassUnderTest;
   URpaiState* GivenState;
END_DEFINE_SPEC(ReasonablePlanningDistanceRotatorSpec)
void ReasonablePlanningDistanceRotatorSpec::Define()
{
   Describe("Calculating Distance for rotator type as an absolute value of manhattan distance", [this]()
      {
         BeforeEach([this]()
            {
               auto MapState = NewObject<URpaiState_Map>();
               MapState->SetAsDynamic(true);

               ClassUnderTest = NewObject<URpaiDistance_Rotator>();
               GivenState = MapState;
            });

         It("Should return the difference from LHS to RHS", [this]()
            {
               const FRotator GivenLHS = FRotator(270.f, 180.f, 90.f);
               const FRotator GivenRHS = FRotator(180.f, -45.0f, 33.3f);

               ClassUnderTest->SetLHS("LHS", EStatePropertyType::Rotator);
               GivenState->SetRotator("LHS", GivenLHS);
               ClassUnderTest->SetRHS(GivenRHS);
               TestEqual("URpaiDistance_Float::CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), FMath::Abs<float>(GivenLHS.GetManhattanDistance(GivenRHS)));
            });

         It("Should return max value if the state value is a float", [this]()
            {
               ClassUnderTest->SetLHS("LHS", EStatePropertyType::Float);
               TestEqual("URpaiDistance_Bool:::CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), TNumericLimits<float>::Max());
            });

         It("Should return max value if the state value is a integer", [this]()
            {
               ClassUnderTest->SetLHS("LHS", EStatePropertyType::Int);
               TestEqual("URpaiDistance_Bool:::CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), TNumericLimits<float>::Max());
            });


         It("Should return max value if the state value is a class", [this]()
            {
               ClassUnderTest->SetLHS("LHS", EStatePropertyType::Class);
               TestEqual("URpaiDistance_Bool:::CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), TNumericLimits<float>::Max());
            });

         It("Should return max value if the state value is a enum", [this]()
            {
               ClassUnderTest->SetLHS("LHS", EStatePropertyType::Enum);
               TestEqual("URpaiDistance_Bool:::CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), TNumericLimits<float>::Max());
            });

         It("Should return max value if the state value is a bool", [this]()
            {
               ClassUnderTest->SetLHS("LHS", EStatePropertyType::Bool);
               TestEqual("URpaiDistance_Bool:::CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), TNumericLimits<float>::Max());
            });

         It("Should return max value if the state value is a name", [this]()
            {
               ClassUnderTest->SetLHS("LHS", EStatePropertyType::Name);
               TestEqual("URpaiDistance_Bool:::CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), TNumericLimits<float>::Max());
            });

         It("Should return max value if the state value is a object", [this]()
            {
               ClassUnderTest->SetLHS("LHS", EStatePropertyType::Object);
               TestEqual("URpaiDistance_Bool:::CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), TNumericLimits<float>::Max());

            });

         It("Should return max value if the state value is a string", [this]()
            {
               ClassUnderTest->SetLHS("LHS", EStatePropertyType::String);
               TestEqual("URpaiDistance_Bool:::CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), TNumericLimits<float>::Max());
            });

         It("Should return max value if the state value is a vector", [this]()
            {
               ClassUnderTest->SetLHS("LHS", EStatePropertyType::Vector);
               TestEqual("URpaiDistance_Bool:::CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), TNumericLimits<float>::Max());
            });

         AfterEach([this]()
            {
               ClassUnderTest->ConditionalBeginDestroy();
               GivenState->ConditionalBeginDestroy();
            });
      });
}

BEGIN_DEFINE_SPEC(ReasonablePlanningDistanceVectorSpec, "ReasonablePlanningAI.Distance.Vector", EAutomationTestFlags::ProductFilter | EAutomationTestFlags_ApplicationContextMask)
   URpaiDistance_Vector* ClassUnderTest;
   URpaiState* GivenState;
END_DEFINE_SPEC(ReasonablePlanningDistanceVectorSpec)
void ReasonablePlanningDistanceVectorSpec::Define()
{
   Describe("Calculating Distance for rotator type as an absolute value of distance squared", [this]()
      {
         BeforeEach([this]()
            {
               auto MapState = NewObject<URpaiState_Map>();
               MapState->SetAsDynamic(true);

               ClassUnderTest = NewObject<URpaiDistance_Vector>();
               GivenState = MapState;
            });

         It("Should return the squared distances absolute value from LHS to RHS", [this]()
            {
               const FVector GivenLHS(0.84f, 10.f, 9.9999f);
               const FVector GivenRHS(-0.454545f, -987.f, 6.f);

               ClassUnderTest->SetLHS("LHS", EStatePropertyType::Vector);
               GivenState->SetVector("LHS", GivenLHS);
               ClassUnderTest->SetRHS(GivenRHS);
               TestEqual("URpaiDistance_Float::CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), FMath::Abs<float>(FVector::DistSquared(GivenLHS, GivenRHS)));
            });

         It("Should return max value if the state value is a float", [this]()
            {
               ClassUnderTest->SetLHS("LHS", EStatePropertyType::Float);
               TestEqual("URpaiDistance_Bool:::CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), TNumericLimits<float>::Max());
            });

         It("Should return max value if the state value is a integer", [this]()
            {
               ClassUnderTest->SetLHS("LHS", EStatePropertyType::Int);
               TestEqual("URpaiDistance_Bool:::CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), TNumericLimits<float>::Max());
            });


         It("Should return max value if the state value is a class", [this]()
            {
               ClassUnderTest->SetLHS("LHS", EStatePropertyType::Class);
               TestEqual("URpaiDistance_Bool:::CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), TNumericLimits<float>::Max());
            });

         It("Should return max value if the state value is a enum", [this]()
            {
               ClassUnderTest->SetLHS("LHS", EStatePropertyType::Enum);
               TestEqual("URpaiDistance_Bool:::CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), TNumericLimits<float>::Max());
            });

         It("Should return max value if the state value is a bool", [this]()
            {
               ClassUnderTest->SetLHS("LHS", EStatePropertyType::Bool);
               TestEqual("URpaiDistance_Bool:::CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), TNumericLimits<float>::Max());
            });

         It("Should return max value if the state value is a name", [this]()
            {
               ClassUnderTest->SetLHS("LHS", EStatePropertyType::Name);
               TestEqual("URpaiDistance_Bool:::CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), TNumericLimits<float>::Max());
            });

         It("Should return max value if the state value is a object", [this]()
            {
               ClassUnderTest->SetLHS("LHS", EStatePropertyType::Object);
               TestEqual("URpaiDistance_Bool:::CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), TNumericLimits<float>::Max());

            });

         It("Should return max value if the state value is a rotator", [this]()
            {
               ClassUnderTest->SetLHS("LHS", EStatePropertyType::Rotator);
               TestEqual("URpaiDistance_Bool:::CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), TNumericLimits<float>::Max());
            });

         It("Should return max value if the state value is a string", [this]()
            {
               ClassUnderTest->SetLHS("LHS", EStatePropertyType::String);
               TestEqual("URpaiDistance_Bool:::CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), TNumericLimits<float>::Max());
            });

         AfterEach([this]()
            {
               ClassUnderTest->ConditionalBeginDestroy();
               GivenState->ConditionalBeginDestroy();
            });
      });
}

BEGIN_DEFINE_SPEC(ReasonablePlanningDistanceStateSpec, "ReasonablePlanningAI.Distance.State", EAutomationTestFlags::ProductFilter | EAutomationTestFlags_ApplicationContextMask)
    URpaiDistance_State* ClassUnderTest;
   URpaiState* GivenState;
END_DEFINE_SPEC(ReasonablePlanningDistanceStateSpec)
void ReasonablePlanningDistanceStateSpec::Define()
{
   Describe("Calculating Distance for Specific Types", [this]()
      {
         BeforeEach([this]()
            {
               auto MapState = NewObject<URpaiState_Map>();
               MapState->SetAsDynamic(true);

               ClassUnderTest = NewObject<URpaiDistance_State>();
               GivenState = MapState;
            });

         It("Should return the squared distance of two vectors", [this]()
            {
               FVector LHS(1.f, 0.f, 0.f);
               FVector RHS(5.5f, 2.f, 4.f);
               GivenState->SetVector("LHS", LHS);
               GivenState->SetVector("RHS", RHS);
               ClassUnderTest->SetLHS("LHS", EStatePropertyType::Vector);
               ClassUnderTest->SetRHS("RHS", EStatePropertyType::Vector);
               TestEqual("URpaiDistance_State::CalculateDistance - Vector", ClassUnderTest->CalculateDistance(GivenState), FMath::Abs<float>(FVector::DistSquared(LHS, RHS)));
            });

         It("Should return the difference between two floats", [this]()
            {
               GivenState->SetFloat("LHS", 100.3f);
               GivenState->SetFloat("RHS", 10.f);
               ClassUnderTest->SetLHS("LHS", EStatePropertyType::Float);
               ClassUnderTest->SetRHS("RHS", EStatePropertyType::Float);
               TestEqual("URpaiDistance_State::CalculateDistance - Float", ClassUnderTest->CalculateDistance(GivenState), 90.3f);
            });

         It("Should return the difference between two integers", [this]()
            {
               GivenState->SetInt("LHS", 85);
               GivenState->SetInt("RHS", 30);
               ClassUnderTest->SetLHS("LHS", EStatePropertyType::Int);
               ClassUnderTest->SetRHS("RHS", EStatePropertyType::Int);
               TestEqual("URpaiDistance_State::CalculateDistance - Int", ClassUnderTest->CalculateDistance(GivenState), 55.f);
            });

         It("Should return a 0 or 1 as the difference between two bools", [this]()
            {
               ClassUnderTest->SetLHS("LHS", EStatePropertyType::Bool);
               ClassUnderTest->SetRHS("RHS", EStatePropertyType::Bool);

               GivenState->SetBool("LHS", true);
               GivenState->SetBool("RHS", false);
               TestEqual("URpaiDistance_State::CalculateDistance - Bool", ClassUnderTest->CalculateDistance(GivenState), 1.f);

               GivenState->SetBool("LHS", true);
               GivenState->SetBool("RHS", true);
               TestEqual("URpaiDistance_State::CalculateDistance - Bool", ClassUnderTest->CalculateDistance(GivenState), 0.f);

               GivenState->SetBool("LHS", false);
               GivenState->SetBool("RHS", false);
               TestEqual("URpaiDistance_State::CalculateDistance - Bool", ClassUnderTest->CalculateDistance(GivenState), 0.f);

               GivenState->SetBool("LHS", false);
               GivenState->SetBool("RHS", true);
               TestEqual("URpaiDistance_State::CalculateDistance - Bool", ClassUnderTest->CalculateDistance(GivenState), 1.f);
            });

         It("Should return the Manhattan distance between two rotators", [this]()
            {
               FRotator LHS(1.f, 0.f, 0.f);
               FRotator RHS(5.5f, 2.f, 4.f);
               GivenState->SetRotator("LHS", LHS);
               GivenState->SetRotator("RHS", RHS);
               ClassUnderTest->SetLHS("LHS", EStatePropertyType::Rotator);
               ClassUnderTest->SetRHS("RHS", EStatePropertyType::Rotator);
               TestEqual("CalculateDistance - FRotator", ClassUnderTest->CalculateDistance(GivenState), FMath::Abs<float>(LHS.GetManhattanDistance(RHS)));
            });

         AfterEach([this]()
            {
               ClassUnderTest->ConditionalBeginDestroy();
               GivenState->ConditionalBeginDestroy();
            });
      });

   Describe("Return Zero on specific scenarios", [this]()
      {
         BeforeEach([this]()
            {
               ClassUnderTest = NewObject<URpaiDistance_State>();
               GivenState = NewObject<UTestPlanningState>();
            });

         It("Should return 0 when same bool is referenced", [this]()
            {
               ClassUnderTest->SetLHS(UTestPlanningState::NAME_TheBoolValue, EStatePropertyType::Bool);
               ClassUnderTest->SetRHS(UTestPlanningState::NAME_TheBoolValue, EStatePropertyType::Bool);
               TestEqual("CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), 0.f);
            });

         It("Should return 0 when same float is referenced", [this]()
            {
               ClassUnderTest->SetLHS(UTestPlanningState::NAME_TheFloatValue, EStatePropertyType::Float);
               ClassUnderTest->SetRHS(UTestPlanningState::NAME_TheFloatValue, EStatePropertyType::Float);
               TestEqual("CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), 0.f);
            });

         It("Should return 0 when same int is referenced", [this]()
            {
               ClassUnderTest->SetLHS(UTestPlanningState::NAME_TheIntValue, EStatePropertyType::Int);
               ClassUnderTest->SetRHS(UTestPlanningState::NAME_TheIntValue, EStatePropertyType::Int);
               TestEqual("CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), 0.f);
            });

         It("Should return 0 when same object is referenced", [this]()
            {
               ClassUnderTest->SetLHS(UTestPlanningState::NAME_TheObjectValue, EStatePropertyType::Object);
               ClassUnderTest->SetRHS(UTestPlanningState::NAME_TheObjectValue, EStatePropertyType::Object);
               TestEqual("CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), 0.f);
            });

         It("Should return 0 when same vector is referenced", [this]()
            {
               ClassUnderTest->SetLHS(UTestPlanningState::NAME_TheVectorValue, EStatePropertyType::Vector);
               ClassUnderTest->SetRHS(UTestPlanningState::NAME_TheVectorValue, EStatePropertyType::Vector);
               TestEqual("CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), 0.f);
            });

         It("Should return 0 when same rotator is referenced", [this]()
            {
               ClassUnderTest->SetLHS(UTestPlanningState::NAME_TheRotatorValue, EStatePropertyType::Rotator);
               ClassUnderTest->SetRHS(UTestPlanningState::NAME_TheRotatorValue, EStatePropertyType::Rotator);
               TestEqual("CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), 0.f);
            });

         It("Should return max distance when invalid types are used", [this]()
            {
                    const float ExpectedValue = TNumericLimits<float>::Max();
                
               ClassUnderTest->SetLHS(UTestPlanningState::NAME_TheRotatorValue, EStatePropertyType::Class);
               ClassUnderTest->SetRHS(UTestPlanningState::NAME_TheIntValue, EStatePropertyType::Class);
               TestEqual("CalculateDistance - Class", ClassUnderTest->CalculateDistance(GivenState), ExpectedValue);

               ClassUnderTest->SetLHS(UTestPlanningState::NAME_TheRotatorValue, EStatePropertyType::Enum);
               ClassUnderTest->SetRHS(UTestPlanningState::NAME_TheIntValue, EStatePropertyType::Enum);
               TestEqual("CalculateDistance - Enum", ClassUnderTest->CalculateDistance(GivenState), ExpectedValue);

               ClassUnderTest->SetLHS(UTestPlanningState::NAME_TheRotatorValue, EStatePropertyType::Name);
               ClassUnderTest->SetRHS(UTestPlanningState::NAME_TheIntValue, EStatePropertyType::Name);
               TestEqual("CalculateDistance - Name", ClassUnderTest->CalculateDistance(GivenState), ExpectedValue);

               ClassUnderTest->SetLHS(UTestPlanningState::NAME_TheRotatorValue, EStatePropertyType::String);
               ClassUnderTest->SetRHS(UTestPlanningState::NAME_TheIntValue, EStatePropertyType::String);
               TestEqual("CalculateDistance - String", ClassUnderTest->CalculateDistance(GivenState), ExpectedValue);

               ClassUnderTest->SetLHS(UTestPlanningState::NAME_TheRotatorValue, EStatePropertyType::Invalid);
               ClassUnderTest->SetRHS(UTestPlanningState::NAME_TheIntValue, EStatePropertyType::Invalid);
               TestEqual("CalculateDistance - Invalid", ClassUnderTest->CalculateDistance(GivenState), ExpectedValue);
            });

         AfterEach([this]()
            {
               ClassUnderTest->ConditionalBeginDestroy();
               GivenState->ConditionalBeginDestroy();
            });
      });

}

BEGIN_DEFINE_SPEC(ReasonablePlanningDistanceCurveFloatSpec, "ReasonablePlanningAI.Distance.CurveFloat", EAutomationTestFlags::ProductFilter | EAutomationTestFlags_ApplicationContextMask)
URpaiDistance_CurveFloat* ClassUnderTest;
URpaiDistance_Float* GivenDistance;
URpaiState* GivenState;
UCurveFloat* GivenCurve;
END_DEFINE_SPEC(ReasonablePlanningDistanceCurveFloatSpec)
void ReasonablePlanningDistanceCurveFloatSpec::Define()
{
   Describe("Calculating Distance for float type as an absolute value", [this]()
      {
         BeforeEach([this]()
            {
               auto MapState = NewObject<URpaiState_Map>();
               MapState->SetAsDynamic(true);

               ClassUnderTest = NewObject<URpaiDistance_CurveFloat>();
               GivenCurve = NewObject<UCurveFloat>();
               GivenCurve->CreateCurveFromCSVString("0,0\r\n1,2\r\n");
               ClassUnderTest->SetCurve(GivenCurve);
               GivenDistance = NewObject<URpaiDistance_Float>();
               ClassUnderTest->SetDistance(GivenDistance);
               GivenState = MapState;
            });

         It("should use the distance result from the distance applied to the curve", [this]()
            {
               GivenState->SetFloat("LHS", 2.0f);
               GivenDistance->SetRHS(1.f);
               TestEqual("URpaiDistance_CurveFloat::CalculateDistance", ClassUnderTest->CalculateDistance(GivenState), 2.f);
            });

         

         AfterEach([this]()
            {
               ClassUnderTest->ConditionalBeginDestroy();
               GivenState->ConditionalBeginDestroy();
               GivenCurve->ConditionalBeginDestroy();
               GivenDistance->ConditionalBeginDestroy();
            });
      });
}
