// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "ReasonablePlanningAITestTypes.h"
#include "Reasoners/RpaiReasoner_DualUtility.h"
#include "Reasoners/RpaiReasoner_AbsoluteUtility.h"
#include "States/RpaiState_Map.h"

BEGIN_DEFINE_SPEC(ReasonablePlanningReasonersSpec, "ReasonablePlanningAI.Reasoners", EAutomationTestFlags::ProductFilter | EAutomationTestFlags_ApplicationContextMask)
   URpaiReasonerBase* ClassUnderTest;
   URpaiState* GivenState;
   TArray<URpaiGoalBase*> GivenGoals;
TArray<URpaiActionBase*> GivenActions;
END_DEFINE_SPEC(ReasonablePlanningReasonersSpec)
void ReasonablePlanningReasonersSpec::Define()
{
   Describe("Dual Utility", [this]()
      {
         BeforeEach([this]()
            {
               auto MapState = NewObject<URpaiState_Map>();
               MapState->SetAsDynamic(true);
               MapState->SetValueOfType(UTestGoal::ConditionAKey, 0);
               MapState->SetValueOfType(UTestGoal::ConditionBKey, 0);
               MapState->SetValueOfType(UTestGoal::ConditionCKey, 0);
               MapState->SetAsDynamic(false);

               ClassUnderTest = NewObject<URpaiReasoner_DualUtility>();
               GivenState = MapState;
            });

         Describe("using multiple goals", [this]()
            {
               BeforeEach([this]()
                  {
                     UTestGoal* A = NewObject<UTestGoal>();
                     UTestGoal* B = NewObject<UTestGoal>();
                     UTestGoal* C = NewObject<UTestGoal>();
                     UTestGoal* D = NewObject<UTestGoal>();
                     UTestGoal* E = NewObject<UTestGoal>();

                     A->Weight = 1.f;
                     B->Weight = 2.f;
                     C->Weight = 100000000.f;
                     D->Weight = 2.f;
                     E->Weight = 1.f;

                     GivenGoals = { A, B, C, D, E };
                  });

               It("should select the goal with the highest probability of being selected by weight", [this]()
                  {
                     auto Actual = ClassUnderTest->ReasonNextGoal(GivenGoals, GivenState);
                     auto Expected = GivenGoals[2];
                     TestEqual("URpaiReasoner_DualUtility::ReasonNextGoal", Actual, Expected);
                  });

               AfterEach([this]()
                  {
                     for (auto Goal : GivenGoals)
                     {
                        Goal->ConditionalBeginDestroy();
                     }
                  });
            });

         Describe("having a single goal", [this]()
            {
               BeforeEach([this]()
                  {
                     GivenGoals = {
                        NewObject<UTestGoal>()
                     };
                  });

               It("should just return the only available goal", [this]()
                  {
                     auto Actual = ClassUnderTest->ReasonNextGoal(GivenGoals, GivenState);
                     auto Expected = GivenGoals[0];
                     TestEqual("URpaiReasoner_DualUtility::ReasonNextGoal", Actual, Expected);
                  });

               AfterEach([this]()
                  {
                     for (auto Goal : GivenGoals)
                     {
                        Goal->ConditionalBeginDestroy();
                     }
                  });
            });

         Describe("having no goals", [this]()
            {
               BeforeEach([this]()
                  {
                     GivenGoals.Empty();
                  });

               It("should return null", [this]()
                  {
                     TestEqual("URpaiReasoner_DualUtility::ReasonNextGoal", ClassUnderTest->ReasonNextGoal(GivenGoals, GivenState), (URpaiGoalBase*)nullptr);
                  });
            });

         AfterEach([this]()
            {
               ClassUnderTest->ConditionalBeginDestroy();
               GivenState->ConditionalBeginDestroy();
            });
      });


   Describe("Absolute Utility", [this]()
      {
         BeforeEach([this]()
            {
               auto MapState = NewObject<URpaiState_Map>();
               MapState->SetAsDynamic(true);
               MapState->SetValueOfType(UTestGoal::ConditionAKey, 0);
               MapState->SetValueOfType(UTestGoal::ConditionBKey, 0);
               MapState->SetValueOfType(UTestGoal::ConditionCKey, 0);
               MapState->SetAsDynamic(false);

               ClassUnderTest = NewObject<URpaiReasoner_AbsoluteUtility>();
               GivenState = MapState;
            });

         Describe("using weight", [this]()
            {
               Describe("using multiple goals", [this]()
                  {
                     BeforeEach([this]()
                        {
                           UTestGoal* A = NewObject<UTestGoal>();
                           UTestGoal* B = NewObject<UTestGoal>();
                           UTestGoal* C = NewObject<UTestGoal>();
                           UTestGoal* D = NewObject<UTestGoal>();
                           UTestGoal* E = NewObject<UTestGoal>();

                           A->Weight = 1.f;
                           B->Weight = 3.f;
                           C->Weight = 0.5f;
                           D->Weight = 2.f;
                           E->Weight = 1.f;

                           GivenGoals = { A, B, C, D, E };
                        });

                     It("should select the goal with the highest weight", [this]()
                        {
                           TestEqual("URpaiReasoner_AbsoluteUtility::ReasonNextGoal", ClassUnderTest->ReasonNextGoal(GivenGoals, GivenState), GivenGoals[1]);
                        });

                     AfterEach([this]()
                        {
                           for (auto Goal : GivenGoals)
                           {
                              Goal->ConditionalBeginDestroy();
                           }
                        });
                  });

               Describe("having a single goal", [this]()
                  {
                     BeforeEach([this]()
                        {
                           GivenGoals = {
                              NewObject<UTestGoal>()
                           };
                        });

                     It("should just return the only available goal", [this]()
                        {
                           auto Actual = ClassUnderTest->ReasonNextGoal(GivenGoals, GivenState);
                           auto Expected = GivenGoals[0];
                           TestEqual("URpaiReasoner_AbsoluteUtility::ReasonNextGoal", Actual, Expected);
                        });

                     AfterEach([this]()
                        {
                           for (auto Goal : GivenGoals)
                           {
                              Goal->ConditionalBeginDestroy();
                           }
                        });
                  });

               Describe("having no goals", [this]()
                  {
                     BeforeEach([this]()
                        {
                           GivenGoals.Empty();
                        });

                     It("should return null", [this]()
                        {
                           TestEqual("URpaiReasoner_AbsoluteUtility::ReasonNextGoal", ClassUnderTest->ReasonNextGoal(GivenGoals, GivenState), (URpaiGoalBase*)nullptr);
                        });
                  });
            });

         AfterEach([this]()
            {
               ClassUnderTest->ConditionalBeginDestroy();
               GivenState->ConditionalBeginDestroy();
            });
      });
}