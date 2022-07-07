#include "Misc/AutomationTest.h"
#include "ReasonablePlanningAITestTypes.h"
#include "Planners/Planner_AStar.h"
#include "States/State_Map.h"

BEGIN_DEFINE_SPEC(ReasonablePlanningPlannerAStarSpec, "ReasonablePlanningAI.Planners", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
	UReasonablePlanningPlannerBase* ClassUnderTest;
	UReasonablePlanningState* GivenState;
	UReasonablePlanningGoalBase* GivenGoal;
	TArray<UReasonablePlanningActionBase*> GivenActions;
END_DEFINE_SPEC(ReasonablePlanningPlannerAStarSpec)
void ReasonablePlanningPlannerAStarSpec::Define()
{
	Describe("A* Planner", [this]()
		{
			BeforeEach([this]()
				{
					auto MapState = NewObject<UState_Map>();
					MapState->SetAsDynamic(true);
					MapState->SetValueOfType(UTestGoal::ConditionAKey, 0);
					MapState->SetValueOfType(UTestGoal::ConditionBKey, 0);
					MapState->SetValueOfType(UTestGoal::ConditionCKey, 0);
					MapState->SetAsDynamic(false);

					ClassUnderTest = NewObject<UPlanner_AStar>();
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

							TArray<UReasonablePlanningActionBase*> Expected({});
							TArray<UReasonablePlanningActionBase*> Actual;

							TestFalse("UPlanner_AStar::PlanChosenGoal", ClassUnderTest->PlanChosenGoal(GivenGoal, GivenState, GivenActions, Actual));
							TestEqual("Planned Actions", Actual, Expected);
						});

					It("should not return a partially satisfied plan", [this]()
						{
							TArray<UReasonablePlanningActionBase*> Expected({});
							TArray<UReasonablePlanningActionBase*> Actual;

							TestFalse("UPlanner_AStar::PlanChosenGoal", ClassUnderTest->PlanChosenGoal(GivenGoal, GivenState, GivenActions, Actual));
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

							TArray<UReasonablePlanningActionBase*> Expected({});
							TArray<UReasonablePlanningActionBase*> Actual;

							TestFalse("UPlanner_AStar::PlanChosenGoal", ClassUnderTest->PlanChosenGoal(GivenGoal, GivenState, GivenActions, Actual));
							TestEqual("Planned Actions", Actual, Expected);
						});

					It("should return an ordered plan based on weights if conditions are satisfied wholly", [this]()
						{
							auto TestActionA = Cast<UTestAction>(GivenActions[0]);
							auto TestActionB = Cast<UTestAction>(GivenActions[1]);
							auto TestActionC = Cast<UTestAction>(GivenActions[2]);
							auto TestActionD = Cast<UTestAction>(GivenActions[3]);

							TestActionA->Weight = 4.f;
							TestActionA->KeyToApply = UTestGoal::ConditionAKey;
							TestActionA->ValueToApply = UTestGoal::ConditionAValue;

							TestActionB->Weight = 8.f;
							TestActionB->KeyToApply = UTestGoal::ConditionAKey;
							TestActionB->ValueToApply = UTestGoal::ConditionAValue + 1;

							TestActionC->KeyToApply = UTestGoal::ConditionBKey;
							TestActionC->ValueToApply = UTestGoal::ConditionBValue;

							TestActionD->KeyToApply = UTestGoal::ConditionCKey;
							TestActionD->ValueToApply = UTestGoal::ConditionCValue;

							TArray<UReasonablePlanningActionBase*> Expected({ GivenActions[0], GivenActions[2], GivenActions[3] });
							TArray<UReasonablePlanningActionBase*> Actual;

							TestTrue("UPlanner_AStar::PlanChosenGoal", ClassUnderTest->PlanChosenGoal(GivenGoal, GivenState, GivenActions, Actual));
							TestEqual("Planned Actions", Actual, Expected);
						});

					It("should return a plan that incrementally satisfies with ordered defined as per the distance algorithm", [this]()
						{
							auto TestActionA = Cast<UTestAction>(GivenActions[0]);
							auto TestActionB = Cast<UTestAction>(GivenActions[1]);
							auto TestActionC = Cast<UTestAction>(GivenActions[2]);
							auto TestActionD = Cast<UTestAction>(GivenActions[3]);

							TestActionA->Weight = 1.f;
							TestActionA->KeyToApply = UTestGoal::ConditionCKey;
							TestActionA->ValueToApply = 1;

							TestActionB->Weight = 2.f;
							TestActionB->KeyToApply = UTestGoal::ConditionAKey;
							TestActionB->ValueToApply = UTestGoal::ConditionAValue;

							TestActionC->KeyToApply = UTestGoal::ConditionBKey;
							TestActionC->ValueToApply = UTestGoal::ConditionBValue;

							TestActionD->Weight = 6.f;
							TestActionD->KeyToApply = UTestGoal::ConditionCKey;
							TestActionD->ValueToApply = 5;

							TArray<UReasonablePlanningActionBase*> Expected({ GivenActions[1], GivenActions[3], GivenActions[2] });
							TArray<UReasonablePlanningActionBase*> Actual;

							TestTrue("UPlanner_AStar::PlanChosenGoal", ClassUnderTest->PlanChosenGoal(GivenGoal, GivenState, GivenActions, Actual));
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