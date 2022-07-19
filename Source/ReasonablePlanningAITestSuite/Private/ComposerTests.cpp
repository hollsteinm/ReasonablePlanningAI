#include "Misc/AutomationTest.h"
#include "ReasonablePlanningAITestTypes.h"
#include "States/State_Map.h"
#include "Planners/Planner_AStar.h"
#include "Reasoners/Reasoner_DualUtility.h"
#include "Reasoners/Reasoner_AbsoluteUtility.h"
#include "Composer/ReasonablePlanningAction.h"
#include "Composer/ReasonablePlanningGoal.h"
#include "Composer/Distances/Distance_State.h"
#include "Composer/Distances/Distance_Bool.h"
#include "Composer/Distances/Distance_Float.h"
#include "Composer/Distances/Distance_Integer.h"
#include "Composer/Distances/Distance_Rotator.h"
#include "Composer/Distances/Distance_Vector.h"
#include "Composer/Mutators/StateMutator_AddFloat.h"
#include "Composer/Mutators/StateMutator_AddInteger.h"
#include "Composer/Mutators/StateMutator_MultiplyFloat.h"
#include "Composer/Mutators/StateMutator_MultiplyInteger.h"
#include "Composer/Mutators/StateMutator_SetValueBool.h"
#include "Composer/Mutators/StateMutator_SetValueClass.h"
#include "Composer/Mutators/StateMutator_SetValueFloat.h"
#include "Composer/Mutators/StateMutator_SetValueInteger.h"
#include "Composer/Mutators/StateMutator_SetValueName.h"
#include "Composer/Mutators/StateMutator_SetValueRotator.h"
#include "Composer/Mutators/StateMutator_SetValueString.h"
#include "Composer/Mutators/StateMutator_SetValueVector.h"
#include "Composer/Queries/StateQuery_CompareDistanceFloat.h"
#include "Composer/Queries/StateQuery_CompareToBool.h"
#include "Composer/Queries/StateQuery_CompareToDistance.h"
#include "Composer/Queries/StateQuery_CompareToFloat.h"
#include "Composer/Queries/StateQuery_CompareToInteger.h"
#include "Composer/Queries/StateQuery_CompareToStateValue.h"
#include "Composer/Queries/StateQuery_Every.h"
#include "Composer/Queries/StateQuery_Any.h"
#include "Composer/Weights/Weight_Float.h"
#include "Composer/Weights/Weight_CurveFloat.h"
#include "Composer/Weights/Weight_ConstantFloat.h"

BEGIN_DEFINE_SPEC(ReasonablePlanningComposerSpec, "ReasonablePlanningAI.Composer", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
	UReasonablePlanningPlannerBase* GivenPlanner;
	UReasonablePlanningReasonerBase* GivenReasoner;
	UReasonablePlanningState* GivenState;
	TArray<UReasonablePlanningGoalBase*> GivenGoals;
	TArray<UReasonablePlanningActionBase*> GivenActions;
	UObject* TransientRoot;
END_DEFINE_SPEC(ReasonablePlanningComposerSpec)
void ReasonablePlanningComposerSpec::Define()
{
	Describe("A Lumberjack", [this]()
		{
			BeforeEach([this]()
				{
					TransientRoot = NewObject<UObject>();

					GivenPlanner = NewObject<UPlanner_AStar>();
					GivenReasoner = NewObject<UReasoner_DualUtility>();
					
					UState_Map* LumberjackState = NewObject<UState_Map>();
					LumberjackState->SetAsDynamic(true);
					LumberjackState->SetInt("TreesInTheForest", 100);
					LumberjackState->SetFloat("Satiation", 1.f);
					LumberjackState->SetFloat("Rest", 1.f);
					LumberjackState->SetBool("IsCarryingWood", false);
					LumberjackState->SetBool("IsChoppingWood", false);
					LumberjackState->SetVector("NextTreeLocation", FVector::ZeroVector);
					LumberjackState->SetVector("NextWoodPileLocation", FVector::ZeroVector);
					LumberjackState->SetVector("WoodStoreLocation", FVector::ZeroVector);
					LumberjackState->SetVector("CurrentLocation", FVector::ZeroVector);
					LumberjackState->SetAsDynamic(false);
					GivenState = LumberjackState;

					UReasonablePlanningGoal* GatherWood = NewObject<UReasonablePlanningGoal>();

					// Is Applicable: We can only gather wood if there are trees to gather wood from.
					UStateQuery_CompareToInteger* HasTreesInTheForestQuery = NewObject<UStateQuery_CompareToInteger>(TransientRoot);
					HasTreesInTheForestQuery->SetQueriedState("TreesInTheForest", EStatePropertyType::Int);
					HasTreesInTheForestQuery->SetComparisonOperation(EStateQueryCompareToOperation::GreaterThan);
					HasTreesInTheForestQuery->SetComparisonValue(0);

					GatherWood->SetIsApplicableQuery(HasTreesInTheForestQuery);

					// Is in Desired State: Our actions have caused us to maximally be near a tree, chop wood, and carry wood, and be near the wood store location.
					// we are done when we are no longer carrying wood, no longer chopping, and are near our wood drop off location. We will also need to add a gate/guard
					// that lets us repeat this action if we start off in a desired state. For instance, immediately after dropping off the wood.
					UStateQuery_CompareToBool* IsNotCarryingWoodQuery = NewObject<UStateQuery_CompareToBool>(TransientRoot);
					IsNotCarryingWoodQuery->SetQueriedState("IsCarryingWood", EStatePropertyType::Bool);
					IsNotCarryingWoodQuery->SetComparisonOperation(EStateQueryCompareToOperation::NotEqualTo);
					IsNotCarryingWoodQuery->SetComparisonValue(true);

					UStateQuery_CompareToBool* IsNotChoppingWoodQuery = NewObject<UStateQuery_CompareToBool>(TransientRoot);
					IsNotChoppingWoodQuery->SetQueriedState("IsChoppingWood", EStatePropertyType::Bool);
					IsNotChoppingWoodQuery->SetComparisonOperation(EStateQueryCompareToOperation::NotEqualTo);
					IsNotChoppingWoodQuery->SetComparisonValue(true);

					UDistance_State* WoodStoreDistance = NewObject<UDistance_State>(TransientRoot);
					WoodStoreDistance->SetLHS("WoodStoreLocation", EStatePropertyType::Vector);
					WoodStoreDistance->SetRHS("CurrentLocation", EStatePropertyType::Vector);

					UStateQuery_CompareDistanceFloat* CloseToWoodStoreQuery = NewObject<UStateQuery_CompareDistanceFloat>(TransientRoot);
					CloseToWoodStoreQuery->SetDistance(WoodStoreDistance);
					CloseToWoodStoreQuery->SetComparisonOperation(EStateQueryCompareToOperation::LessThanOrEqualTo);
					CloseToWoodStoreQuery->SetRHS(90000.f);

					UStateQuery_Every* IsInDesiredStateQuery = NewObject<UStateQuery_Every>(TransientRoot);
					IsInDesiredStateQuery->SetSubQueries({ IsNotCarryingWoodQuery, IsNotChoppingWoodQuery, CloseToWoodStoreQuery });

					GatherWood->SetIsInDesiredStateQuery(IsInDesiredStateQuery);

					// Our distance is based on whether we have any wood, we need to chop down a tree, and if we are actively chopping, as well as if we dropped off wood.

					UReasonablePlanningGoal* SustainSelf = NewObject<UReasonablePlanningGoal>();
					UReasonablePlanningGoal* PreserveForest = NewObject<UReasonablePlanningGoal>();

					GivenGoals = {
						GatherWood,
						SustainSelf,
						PreserveForest
					};

				});

			AfterEach([this]()
				{
					TransientRoot->ConditionalBeginDestroy();

					for (auto Action : GivenActions)
					{
						Action->ConditionalBeginDestroy();
					}

					for (auto Goal : GivenGoals)
					{
						Goal->ConditionalBeginDestroy();
					}

					GivenPlanner->ConditionalBeginDestroy();
					GivenReasoner->ConditionalBeginDestroy();
					GivenState->ConditionalBeginDestroy();
				});
		});
}