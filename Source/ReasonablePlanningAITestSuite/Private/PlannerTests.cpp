// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "ReasonablePlanningAITestTypes.h"
#include "Planners/RpaiPlanner_AStar.h"
#include "Planners/RpaiPlanner_HUG.h"
#include "States/RpaiState_Map.h"

BEGIN_DEFINE_SPEC(ReasonablePlanningPlannerImplSpec, "ReasonablePlanningAI.Planners", EAutomationTestFlags::ProductFilter | EAutomationTestFlags_ApplicationContextMask)
   URpaiPlannerBase* ClassUnderTest;
   URpaiState* GivenState;
   URpaiGoalBase* GivenGoal;
   TArray<URpaiActionBase*> GivenActions;
END_DEFINE_SPEC(ReasonablePlanningPlannerImplSpec)
void ReasonablePlanningPlannerImplSpec::Define()
{
   Describe("A* Planner", [this]()
      {
         BeforeEach([this]()
            {
               auto MapState = NewObject<URpaiState_Map>();
               MapState->SetAsDynamic(true);
               MapState->SetValueOfType(UTestGoal::ConditionAKey, 0);
               MapState->SetValueOfType(UTestGoal::ConditionBKey, 0);
               MapState->SetValueOfType(UTestGoal::ConditionCKey, 0);
               MapState->SetAsDynamic(false);

               ClassUnderTest = NewObject<URpaiPlanner_AStar>();
               GivenGoal = NewObject<UTestGoal>();
               GivenState = MapState;
            });

         Describe("With only one available action", [this]()
            {
               BeforeEach([this]()
                  {
                     auto TestAction = NewObject<UTestAction>();
                     GivenActions = {
                        TestAction
                     };
                  });

               It("should not return non-applicable action", [this]()
                  {
                     Cast<UTestAction>(GivenActions[0])->IsApplicable = false;

                     TArray<URpaiActionBase*> Expected({});
                     TArray<URpaiActionBase*> Actual;

                     TestFalse("URpaiPlanner_AStar::PlanChosenGoal", ClassUnderTest->PlanChosenGoal(GivenGoal, GivenState, GivenActions, Actual));
                     TestEqual("Planned Actions", Actual, Expected);
                  });

               It("should not return a partially satisfied plan", [this]()
                  {
                     TArray<URpaiActionBase*> Expected({});
                     TArray<URpaiActionBase*> Actual;

                     TestFalse("URpaiPlanner_AStar::PlanChosenGoal", ClassUnderTest->PlanChosenGoal(GivenGoal, GivenState, GivenActions, Actual));
                     TestEqual("Planned Actions", Actual, Expected);
                  });

               AfterEach([this]()
                  {
                     for (auto Action : GivenActions)
                     {
                        Action->ConditionalBeginDestroy();
                     }
                  });
            });

         Describe("With a set of actions given", [this]()
            {
               BeforeEach([this]()
                  {
                     auto TestActionA = NewObject<UTestAction>();
                     auto TestActionB = NewObject<UTestAction>();
                     auto TestActionC = NewObject<UTestAction>();
                     auto TestActionD = NewObject<UTestAction>();
                     GivenActions = {
                        TestActionA,
                        TestActionB,
                        TestActionC,
                        TestActionD
                     };
                  });

               It("should not return a partially satisfied plan", [this]()
                  {
                     Cast<UTestAction>(GivenActions[0])->IsApplicable = false;
                     Cast<UTestAction>(GivenActions[3])->IsApplicable = false;

                     TArray<URpaiActionBase*> Expected({});
                     TArray<URpaiActionBase*> Actual;

                     TestFalse("URpaiPlanner_AStar::PlanChosenGoal", ClassUnderTest->PlanChosenGoal(GivenGoal, GivenState, GivenActions, Actual));
                     TestEqual("Planned Actions", Actual, Expected);
                  });

               It("should return an ordered plan based on weights if conditions are satisfied wholly", [this]()
                  {
                     auto TestGoal = Cast<UTestGoal>(GivenGoal);
                     auto TestActionA = Cast<UTestAction>(GivenActions[0]);
                     auto TestActionB = Cast<UTestAction>(GivenActions[1]);
                     auto TestActionC = Cast<UTestAction>(GivenActions[2]);
                     auto TestActionD = Cast<UTestAction>(GivenActions[3]);

                     TestActionA->Weight = 4.f;
                     TestActionA->KeyToApply = UTestGoal::ConditionAKey;
                     TestActionA->ValueToApply = TestGoal->ConditionAValue;

                     TestActionB->Weight = 8.f;
                     TestActionB->KeyToApply = UTestGoal::ConditionAKey;
                     TestActionB->ValueToApply = TestGoal->ConditionAValue + 1;

                     TestActionC->KeyToApply = UTestGoal::ConditionBKey;
                     TestActionC->ValueToApply = TestGoal->ConditionBValue;

                     TestActionD->KeyToApply = UTestGoal::ConditionCKey;
                     TestActionD->ValueToApply = TestGoal->ConditionCValue;

                     TArray<URpaiActionBase*> Expected({ GivenActions[0], GivenActions[2], GivenActions[3] });
                     TArray<URpaiActionBase*> Actual;

                     TestTrue("URpaiPlanner_AStar::PlanChosenGoal", ClassUnderTest->PlanChosenGoal(GivenGoal, GivenState, GivenActions, Actual));
                     TestEqual("Planned Actions", Actual, Expected);
                  });

               It("should return a plan that incrementally satisfies with ordered defined as per the distance algorithm", [this]()
                  {
                     auto TestGoal = Cast<UTestGoal>(GivenGoal);
                     auto TestActionA = Cast<UTestAction>(GivenActions[0]);
                     auto TestActionB = Cast<UTestAction>(GivenActions[1]);
                     auto TestActionC = Cast<UTestAction>(GivenActions[2]);
                     auto TestActionD = Cast<UTestAction>(GivenActions[3]);

                     TestActionA->Weight = 1.f;
                     TestActionA->KeyToApply = UTestGoal::ConditionCKey;
                     TestActionA->ValueToApply = 1;

                     TestActionB->Weight = 2.f;
                     TestActionB->KeyToApply = UTestGoal::ConditionAKey;
                     TestActionB->ValueToApply = TestGoal->ConditionAValue;

                     TestActionC->KeyToApply = UTestGoal::ConditionBKey;
                     TestActionC->ValueToApply = TestGoal->ConditionBValue;

                     TestActionD->Weight = 6.f;
                     TestActionD->KeyToApply = UTestGoal::ConditionCKey;
                     TestActionD->ValueToApply = 5;

                     TArray<URpaiActionBase*> Expected({ GivenActions[1], GivenActions[2], GivenActions[3]});
                     TArray<URpaiActionBase*> Actual;

                     TestTrue("URpaiPlanner_AStar::PlanChosenGoal", ClassUnderTest->PlanChosenGoal(GivenGoal, GivenState, GivenActions, Actual));
                     TestEqual("Planned Actions", Actual, Expected);
                  });

               AfterEach([this]()
                  {
                     for (auto Action : GivenActions)
                     {
                        Action->ConditionalBeginDestroy();
                     }
                  });
            });

         AfterEach([this]()
            {
               ClassUnderTest->ConditionalBeginDestroy();
               GivenState->ConditionalBeginDestroy();
               GivenGoal->ConditionalBeginDestroy();
            });
      });

   Describe("HUG Planner", [this]()
      {
         BeforeEach([this]()
            {
               auto MapState = NewObject<URpaiState_Map>();
               MapState->SetAsDynamic(true);
               MapState->SetValueOfType(UTestGoal::ConditionAKey, 0);
               MapState->SetValueOfType(UTestGoal::ConditionBKey, 0);
               MapState->SetValueOfType(UTestGoal::ConditionCKey, 0);
               MapState->SetAsDynamic(false);

               ClassUnderTest = NewObject<URpaiPlanner_HUG>();
               GivenGoal = NewObject<UTestGoal>();
               GivenState = MapState;
            });

         Describe("With only one available action", [this]()
            {
               BeforeEach([this]()
                  {
                     auto TestAction = NewObject<UTestAction>();
                     GivenActions = {
                        TestAction
                     };
                  });

               It("should not return non-applicable action", [this]()
                  {
                     Cast<UTestAction>(GivenActions[0])->IsApplicable = false;

                     TArray<URpaiActionBase*> Expected({});
                     TArray<URpaiActionBase*> Actual;

                     TestFalse("URpaiPlanner_AStar::PlanChosenGoal", ClassUnderTest->PlanChosenGoal(GivenGoal, GivenState, GivenActions, Actual));
                     TestEqual("Planned Actions", Actual, Expected);
                  });

               It("should not return a partially satisfied plan", [this]()
                  {
                     TArray<URpaiActionBase*> Expected({});
                     TArray<URpaiActionBase*> Actual;

                     TestFalse("URpaiPlanner_AStar::PlanChosenGoal", ClassUnderTest->PlanChosenGoal(GivenGoal, GivenState, GivenActions, Actual));
                     TestEqual("Planned Actions", Actual, Expected);
                  });

               AfterEach([this]()
                  {
                     for (auto Action : GivenActions)
                     {
                        Action->ConditionalBeginDestroy();
                     }
                  });
            });

         Describe("With a set of actions given", [this]()
            {
               BeforeEach([this]()
                  {
                     auto TestActionA = NewObject<UTestAction>();
                     auto TestActionB = NewObject<UTestAction>();
                     auto TestActionC = NewObject<UTestAction>();
                     auto TestActionD = NewObject<UTestAction>();
                     GivenActions = {
                        TestActionA,
                        TestActionB,
                        TestActionC,
                        TestActionD
                     };
                  });

               It("should not return a partially satisfied plan", [this]()
                  {
                     Cast<UTestAction>(GivenActions[0])->IsApplicable = false;
                     Cast<UTestAction>(GivenActions[3])->IsApplicable = false;

                     TArray<URpaiActionBase*> Expected({});
                     TArray<URpaiActionBase*> Actual;

                     TestFalse("URpaiPlanner_AStar::PlanChosenGoal", ClassUnderTest->PlanChosenGoal(GivenGoal, GivenState, GivenActions, Actual));
                     TestEqual("Planned Actions", Actual, Expected);
                  });

               It("should return an ordered plan based on weights if conditions are satisfied wholly", [this]()
                  {
                     auto TestGoal = Cast<UTestGoal>(GivenGoal);
                     auto TestActionA = Cast<UTestAction>(GivenActions[0]);
                     auto TestActionB = Cast<UTestAction>(GivenActions[1]);
                     auto TestActionC = Cast<UTestAction>(GivenActions[2]);
                     auto TestActionD = Cast<UTestAction>(GivenActions[3]);

                     TestActionA->Weight = 4.f;
                     TestActionA->KeyToApply = UTestGoal::ConditionAKey;
                     TestActionA->ValueToApply = TestGoal->ConditionAValue;

                     TestActionB->Weight = 8.f;
                     TestActionB->KeyToApply = UTestGoal::ConditionAKey;
                     TestActionB->ValueToApply = TestGoal->ConditionAValue + 1;

                     TestActionC->KeyToApply = UTestGoal::ConditionBKey;
                     TestActionC->ValueToApply = TestGoal->ConditionBValue;

                     TestActionD->KeyToApply = UTestGoal::ConditionCKey;
                     TestActionD->ValueToApply = TestGoal->ConditionCValue;

                     TArray<URpaiActionBase*> Expected({ GivenActions[0], GivenActions[2], GivenActions[3] });
                     TArray<URpaiActionBase*> Actual;

                     TestTrue("URpaiPlanner_AStar::PlanChosenGoal", ClassUnderTest->PlanChosenGoal(GivenGoal, GivenState, GivenActions, Actual));
                     TestEqual("Planned Actions", Actual, Expected);
                  });

               It("should return a plan that incrementally satisfies with ordered defined as per the distance algorithm", [this]()
                  {
                     auto TestGoal = Cast<UTestGoal>(GivenGoal);
                     auto TestActionA = Cast<UTestAction>(GivenActions[0]);
                     auto TestActionB = Cast<UTestAction>(GivenActions[1]);
                     auto TestActionC = Cast<UTestAction>(GivenActions[2]);
                     auto TestActionD = Cast<UTestAction>(GivenActions[3]);

                     TestActionA->Weight = 1.f;
                     TestActionA->KeyToApply = UTestGoal::ConditionCKey;
                     TestActionA->ValueToApply = 1;

                     TestActionB->Weight = 2.f;
                     TestActionB->KeyToApply = UTestGoal::ConditionAKey;
                     TestActionB->ValueToApply = TestGoal->ConditionAValue;

                     TestActionC->KeyToApply = UTestGoal::ConditionBKey;
                     TestActionC->ValueToApply = TestGoal->ConditionBValue;

                     TestActionD->Weight = 6.f;
                     TestActionD->KeyToApply = UTestGoal::ConditionCKey;
                     TestActionD->ValueToApply = 5;

                     TArray<URpaiActionBase*> Expected({ GivenActions[1], GivenActions[2], GivenActions[3] });
                     TArray<URpaiActionBase*> Actual;

                     TestTrue("URpaiPlanner_AStar::PlanChosenGoal", ClassUnderTest->PlanChosenGoal(GivenGoal, GivenState, GivenActions, Actual));
                     TestEqual("Planned Actions", Actual, Expected);
                  });

               AfterEach([this]()
                  {
                     for (auto Action : GivenActions)
                     {
                        Action->ConditionalBeginDestroy();
                     }
                  });
            });

         AfterEach([this]()
            {
               ClassUnderTest->ConditionalBeginDestroy();
               GivenState->ConditionalBeginDestroy();
               GivenGoal->ConditionalBeginDestroy();
            });
      });
}