#include "Misc/AutomationTest.h"
#include "ReasonablePlanningAITestTypes.h"
#include "Composer/ReasonablePlanningDistance.h"
#include "Composer/Distances/Distance_State.h"
#include "States/State_Map.h"

BEGIN_DEFINE_SPEC(ReasonablePlanningDistanceSpec, "ReasonablePlanningAI.Distance", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
    UDistance_State* ClassUnderTest;
	UReasonablePlanningState* GivenState;
END_DEFINE_SPEC(ReasonablePlanningDistanceSpec)
void ReasonablePlanningDistanceSpec::Define()
{
	Describe("Calculating Distance for Specific Types", [this]()
		{
			BeforeEach([this]()
				{
					auto MapState = NewObject<UState_Map>();
					MapState->SetAsDynamic(true);

					ClassUnderTest = NewObject<UDistance_State>();
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
					TestEqual("CalculateDistance - Vector", ClassUnderTest->CalculateDistance(GivenState), FVector::DistSquared(LHS, RHS));
				});

			It("Should return the difference between two floats", [this]()
				{
					GivenState->SetFloat("LHS", 100.3f);
					GivenState->SetFloat("RHS", 10.f);
					ClassUnderTest->SetLHS("LHS", EStatePropertyType::Float);
					ClassUnderTest->SetRHS("RHS", EStatePropertyType::Float);
					TestEqual("CalculateDistance - Float", ClassUnderTest->CalculateDistance(GivenState), 90.3f);
				});

			It("Should return the difference between two integers", [this]()
				{
					GivenState->SetInt("LHS", 85);
					GivenState->SetInt("RHS", 30);
					ClassUnderTest->SetLHS("LHS", EStatePropertyType::Int);
					ClassUnderTest->SetRHS("RHS", EStatePropertyType::Int);
					TestEqual("CalculateDistance - Int", ClassUnderTest->CalculateDistance(GivenState), 55.f);
				});

			It("Should return a 0 or 1 as the difference between two bools", [this]()
				{
					ClassUnderTest->SetLHS("LHS", EStatePropertyType::Bool);
					ClassUnderTest->SetRHS("RHS", EStatePropertyType::Bool);

					GivenState->SetBool("LHS", true);
					GivenState->SetBool("RHS", false);
					TestEqual("CalculateDistance - Int", ClassUnderTest->CalculateDistance(GivenState), 1.f);

					GivenState->SetBool("LHS", true);
					GivenState->SetBool("RHS", true);
					TestEqual("CalculateDistance - Int", ClassUnderTest->CalculateDistance(GivenState), 0.f);

					GivenState->SetBool("LHS", false);
					GivenState->SetBool("RHS", false);
					TestEqual("CalculateDistance - Int", ClassUnderTest->CalculateDistance(GivenState), 0.f);

					GivenState->SetBool("LHS", false);
					GivenState->SetBool("RHS", true);
					TestEqual("CalculateDistance - Int", ClassUnderTest->CalculateDistance(GivenState), 1.f);
				});

			It("Should return the Manhattan distance between two rotators", [this]()
				{
					FRotator LHS(1.f, 0.f, 0.f);
					FRotator RHS(5.5f, 2.f, 4.f);
					GivenState->SetRotator("LHS", LHS);
					GivenState->SetRotator("RHS", RHS);
					ClassUnderTest->SetLHS("LHS", EStatePropertyType::Rotator);
					ClassUnderTest->SetRHS("RHS", EStatePropertyType::Rotator);
					TestEqual("CalculateDistance - FRotator", ClassUnderTest->CalculateDistance(GivenState), LHS.GetManhattanDistance(RHS));
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
					ClassUnderTest = NewObject<UDistance_State>();
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

			It("Should return 0 when invalid types are used", [this]()
				{
					ClassUnderTest->SetLHS(UTestPlanningState::NAME_TheRotatorValue, EStatePropertyType::Class);
					ClassUnderTest->SetRHS(UTestPlanningState::NAME_TheRotatorValue, EStatePropertyType::Class);
					TestEqual("CalculateDistance - Class", ClassUnderTest->CalculateDistance(GivenState), 0.f);

					ClassUnderTest->SetLHS(UTestPlanningState::NAME_TheRotatorValue, EStatePropertyType::Enum);
					ClassUnderTest->SetRHS(UTestPlanningState::NAME_TheRotatorValue, EStatePropertyType::Enum);
					TestEqual("CalculateDistance - Enum", ClassUnderTest->CalculateDistance(GivenState), 0.f);

					ClassUnderTest->SetLHS(UTestPlanningState::NAME_TheRotatorValue, EStatePropertyType::Name);
					ClassUnderTest->SetRHS(UTestPlanningState::NAME_TheRotatorValue, EStatePropertyType::Name);
					TestEqual("CalculateDistance - Name", ClassUnderTest->CalculateDistance(GivenState), 0.f);

					ClassUnderTest->SetLHS(UTestPlanningState::NAME_TheRotatorValue, EStatePropertyType::String);
					ClassUnderTest->SetRHS(UTestPlanningState::NAME_TheRotatorValue, EStatePropertyType::String);
					TestEqual("CalculateDistance - String", ClassUnderTest->CalculateDistance(GivenState), 0.f);

					ClassUnderTest->SetLHS(UTestPlanningState::NAME_TheRotatorValue, EStatePropertyType::Invalid);
					ClassUnderTest->SetRHS(UTestPlanningState::NAME_TheRotatorValue, EStatePropertyType::Invalid);
					TestEqual("CalculateDistance - Invalid", ClassUnderTest->CalculateDistance(GivenState), 0.f);
				});

			AfterEach([this]()
				{
					ClassUnderTest->ConditionalBeginDestroy();
					GivenState->ConditionalBeginDestroy();
				});
		});

}
