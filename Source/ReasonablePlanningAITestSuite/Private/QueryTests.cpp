#include "Misc/AutomationTest.h"
#include "ReasonablePlanningAITestTypes.h"
#include "Composer/Queries/StateQuery_CompareToBool.h"
#include "Composer/Queries/StateQuery_CompareToFloat.h"
#include "Composer/Queries/StateQuery_CompareToInteger.h"
#include "Composer/Queries/StateQuery_CompareToStateValue.h"

BEGIN_DEFINE_SPEC(ReasonablePlanningQueryCompareToBoolSpec, "ReasonablePlanningAI.Queries.CompareBool", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
	UStateQuery_CompareToBool* ClassUnderTest;
	UTestPlanningState* GivenState;
END_DEFINE_SPEC(ReasonablePlanningQueryCompareToBoolSpec)
void ReasonablePlanningQueryCompareToBoolSpec::Define()
{
	BeforeEach([this]()
		{
			ClassUnderTest = NewObject<UStateQuery_CompareToBool>();
			GivenState = NewObject<UTestPlanningState>();

			ClassUnderTest->SetQueriedState(UTestPlanningState::NAME_TheBoolValue, EStatePropertyType::Bool);
		});

	Describe("Successful queries", [this]()
		{
			It("Should return success when the value defined is less than state value", [this]()
				{
					ClassUnderTest->SetComparisonOperation(EStateQueryCompareToOperation::LessThan);

					GivenState->SetBool(UTestPlanningState::NAME_TheBoolValue, false);
					ClassUnderTest->SetComparisonValue(true);
					TestEqual("UStateQuery_CompareToBool::Query", ClassUnderTest->Query(GivenState), EStateQueryResult::Succeeded);
				});

			It("Should return success when the value defined is less than or equal to state value", [this]()
				{
					ClassUnderTest->SetComparisonOperation(EStateQueryCompareToOperation::LessThanOrEqualTo);

					GivenState->SetBool(UTestPlanningState::NAME_TheBoolValue, false);
					ClassUnderTest->SetComparisonValue(true);
					TestEqual("UStateQuery_CompareToBool::Query", ClassUnderTest->Query(GivenState), EStateQueryResult::Succeeded);

					ClassUnderTest->SetComparisonValue(false);
					TestEqual("UStateQuery_CompareToBool::Query", ClassUnderTest->Query(GivenState), EStateQueryResult::Succeeded);
				});

			It("Should return success when the value defined is greater than state value", [this]()
				{
					ClassUnderTest->SetComparisonOperation(EStateQueryCompareToOperation::GreaterThan);

					GivenState->SetBool(UTestPlanningState::NAME_TheBoolValue, true);
					ClassUnderTest->SetComparisonValue(false);
					TestEqual("UStateQuery_CompareToBool::Query", ClassUnderTest->Query(GivenState), EStateQueryResult::Succeeded);
				});

			It("Should return success when the value defined is greater than or equal to state value", [this]()
				{
					ClassUnderTest->SetComparisonOperation(EStateQueryCompareToOperation::GreaterThanOrEqualTo);

					GivenState->SetBool(UTestPlanningState::NAME_TheBoolValue, true);
					ClassUnderTest->SetComparisonValue(true);
					TestEqual("UStateQuery_CompareToBool::Query", ClassUnderTest->Query(GivenState), EStateQueryResult::Succeeded);

					ClassUnderTest->SetComparisonValue(false);
					TestEqual("UStateQuery_CompareToBool::Query", ClassUnderTest->Query(GivenState), EStateQueryResult::Succeeded);
				});

			It("Should return success when the value defined is equal to the value", [this]()
				{
					ClassUnderTest->SetComparisonOperation(EStateQueryCompareToOperation::EqualTo);

					GivenState->SetBool(UTestPlanningState::NAME_TheBoolValue, true);
					ClassUnderTest->SetComparisonValue(true);
					TestEqual("UStateQuery_CompareToBool::Query", ClassUnderTest->Query(GivenState), EStateQueryResult::Succeeded);
				});

			It("Should return success when the value defined is not equal to the value", [this]()
				{
					ClassUnderTest->SetComparisonOperation(EStateQueryCompareToOperation::NotEqualTo);

					GivenState->SetBool(UTestPlanningState::NAME_TheBoolValue, false);
					ClassUnderTest->SetComparisonValue(true);
					TestEqual("UStateQuery_CompareToBool::Query", ClassUnderTest->Query(GivenState), EStateQueryResult::Succeeded);

					GivenState->SetBool(UTestPlanningState::NAME_TheBoolValue, true);
					ClassUnderTest->SetComparisonValue(false);
					TestEqual("UStateQuery_CompareToBool::Query", ClassUnderTest->Query(GivenState), EStateQueryResult::Succeeded);
				});
		});

	AfterEach([this]()
		{
			ClassUnderTest->ConditionalBeginDestroy();
			GivenState->ConditionalBeginDestroy();
		});
}

BEGIN_DEFINE_SPEC(ReasonablePlanningQueryCompareToFloatSpec, "ReasonablePlanningAI.Queries.CompareBool", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
	UStateQuery_CompareToFloat* ClassUnderTest;
	UTestPlanningState* GivenState;
END_DEFINE_SPEC(ReasonablePlanningQueryCompareToFloatSpec)
void ReasonablePlanningQueryCompareToFloatSpec::Define()
{
	BeforeEach([this]()
		{
			ClassUnderTest = NewObject<UStateQuery_CompareToFloat>();
			GivenState = NewObject<UTestPlanningState>();

			ClassUnderTest->SetQueriedState(UTestPlanningState::NAME_TheFloatValue, EStatePropertyType::Float);
		});

	Describe("Successful queries", [this]()
		{
			It("Should return success when the value defined is less than state value", [this]()
				{
					ClassUnderTest->SetComparisonOperation(EStateQueryCompareToOperation::LessThan);

					GivenState->SetFloat(UTestPlanningState::NAME_TheFloatValue, 100.0f);
					ClassUnderTest->SetComparisonValue(150.f);
					TestEqual("UStateQuery_CompareToFloat::Query", ClassUnderTest->Query(GivenState), EStateQueryResult::Succeeded);
				});

			It("Should return success when the value defined is less than or equal to state value", [this]()
				{
					ClassUnderTest->SetComparisonOperation(EStateQueryCompareToOperation::LessThanOrEqualTo);

					GivenState->SetFloat(UTestPlanningState::NAME_TheFloatValue, -25.f);
					ClassUnderTest->SetComparisonValue(100.f);
					TestEqual("UStateQuery_CompareToFloat::Query", ClassUnderTest->Query(GivenState), EStateQueryResult::Succeeded);

					ClassUnderTest->SetComparisonValue(-25.f);
					TestEqual("UStateQuery_CompareToFloat::Query", ClassUnderTest->Query(GivenState), EStateQueryResult::Succeeded);
				});

			It("Should return success when the value defined is greater than state value", [this]()
				{
					ClassUnderTest->SetComparisonOperation(EStateQueryCompareToOperation::GreaterThan);

					GivenState->SetFloat(UTestPlanningState::NAME_TheFloatValue, -2500.f);
					ClassUnderTest->SetComparisonValue(-100000000.f);
					TestEqual("UStateQuery_CompareToFloat::Query", ClassUnderTest->Query(GivenState), EStateQueryResult::Succeeded);
				});

			It("Should return success when the value defined is greater than or equal to state value", [this]()
				{
					ClassUnderTest->SetComparisonOperation(EStateQueryCompareToOperation::GreaterThanOrEqualTo);

					GivenState->SetFloat(UTestPlanningState::NAME_TheFloatValue, 1.f);
					ClassUnderTest->SetComparisonValue(0.56f);
					TestEqual("UStateQuery_CompareToFloat::Query", ClassUnderTest->Query(GivenState), EStateQueryResult::Succeeded);

					ClassUnderTest->SetComparisonValue(1.f);
					TestEqual("UStateQuery_CompareToFloat::Query", ClassUnderTest->Query(GivenState), EStateQueryResult::Succeeded);
				});

			It("Should return success when the value defined is equal to the value", [this]()
				{
					ClassUnderTest->SetComparisonOperation(EStateQueryCompareToOperation::EqualTo);

					GivenState->SetFloat(UTestPlanningState::NAME_TheFloatValue, 456.789f);
					ClassUnderTest->SetComparisonValue(456.789f);
					TestEqual("UStateQuery_CompareToFloat::Query", ClassUnderTest->Query(GivenState), EStateQueryResult::Succeeded);
				});

			It("Should return success when the value defined is not equal to the value", [this]()
				{
					ClassUnderTest->SetComparisonOperation(EStateQueryCompareToOperation::NotEqualTo);

					GivenState->SetFloat(UTestPlanningState::NAME_TheFloatValue, -456.789f);
					ClassUnderTest->SetComparisonValue(456.789f);
					TestEqual("UStateQuery_CompareToFloat::Query", ClassUnderTest->Query(GivenState), EStateQueryResult::Succeeded);

					GivenState->SetFloat(UTestPlanningState::NAME_TheFloatValue, 456.789f);
					ClassUnderTest->SetComparisonValue(-456.789f);
					TestEqual("UStateQuery_CompareToFloat::Query", ClassUnderTest->Query(GivenState), EStateQueryResult::Succeeded);
				});
		});

	AfterEach([this]()
		{
			ClassUnderTest->ConditionalBeginDestroy();
			GivenState->ConditionalBeginDestroy();
		});
}