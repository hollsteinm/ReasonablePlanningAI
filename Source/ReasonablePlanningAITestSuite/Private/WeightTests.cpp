#include "Misc/AutomationTest.h"
#include "ReasonablePlanningAITestTypes.h"
#include "ReasonablePlanningWeightBase.h"
#include "Weight_Float.h"
#include "Weight_CurveFloat.h"
#include "Weight_ConstantFloat.h"

BEGIN_DEFINE_SPEC(ReasonablePlanningWeightSpec, "ReasonablePlanningAI.Weight", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
	UReasonablePlanningWeight* ClassUnderTest;
	UTestPlanningState* GivenState;
END_DEFINE_SPEC(ReasonablePlanningWeightSpec)
void ReasonablePlanningWeightSpec::Define()
{
	BeforeEach([this]()
		{
			ClassUnderTest = NewObject<UReasonablePlanningWeight>();
			GivenState = NewObject<UTestPlanningState>();
		});

	Describe("Base Weight Behavior Defaults", [this]()
	{
			It("Should return an insignificant weight", [this]()
				{
					TestEqual("ExecutionWeight", ClassUnderTest->ExecutionWeight(GivenState), 0.f);
				});
	});

	AfterEach([this]()
		{
			ClassUnderTest->ConditionalBeginDestroy();
			GivenState->ConditionalBeginDestroy();
		});
}

BEGIN_DEFINE_SPEC(ReasonablePlanningWeightConstantFloatSpec, "ReasonablePlanningAI.WeightConstantFloat", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
	UWeight_ConstantFloat* ClassUnderTest;
	UTestPlanningState* GivenState;
END_DEFINE_SPEC(ReasonablePlanningWeightConstantFloatSpec)
void ReasonablePlanningWeightConstantFloatSpec::Define()
{
	BeforeEach([this]()
		{
			ClassUnderTest = NewObject<UWeight_ConstantFloat>();
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

BEGIN_DEFINE_SPEC(ReasonablePlanningWeightFloatSpec, "ReasonablePlanningAI.WeightFloat", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
	UWeight_Float* ClassUnderTest;
	UTestPlanningState* GivenState;
END_DEFINE_SPEC(ReasonablePlanningWeightFloatSpec)
void ReasonablePlanningWeightFloatSpec::Define()
{
	BeforeEach([this]()
		{
			ClassUnderTest = NewObject<UWeight_Float>();
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

BEGIN_DEFINE_SPEC(ReasonablePlanningWeightCurveFloatSpec, "ReasonablePlanningAI.WeightCurveFloat", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
	UWeight_CurveFloat* ClassUnderTest;
	UTestPlanningState* GivenState;
	UCurveFloat* GivenCurve;
END_DEFINE_SPEC(ReasonablePlanningWeightCurveFloatSpec)
void ReasonablePlanningWeightCurveFloatSpec::Define()
{

	BeforeEach([this]()
		{
			ClassUnderTest = NewObject<UWeight_CurveFloat>();
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