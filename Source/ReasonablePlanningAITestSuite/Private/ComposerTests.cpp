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
#include "Composer/Distances/Distance_AddAll.h"
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
#include "Composer/Mutators/StateMutator_CopyState.h"
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
#include "Composer/Weights/Weight_AddAll.h"
#include "Composer/Weights/Weight_Distance.h"

BEGIN_DEFINE_SPEC(ReasonablePlanningComposerSpec, "ReasonablePlanningAI.Composer", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
	UReasonablePlanningPlannerBase* GivenPlanner;
	UReasonablePlanningReasonerBase* GivenReasoner;
	UReasonablePlanningState* GivenState;
	TArray<UReasonablePlanningGoalBase*> GivenGoals;
	TArray<UReasonablePlanningActionBase*> GivenActions;
END_DEFINE_SPEC(ReasonablePlanningComposerSpec)
void ReasonablePlanningComposerSpec::Define()
{
	Describe("A Lumberjack", [this]()
		{
			BeforeEach([this]()
				{
					GivenPlanner = NewObject<UPlanner_AStar>();
					GivenReasoner = NewObject<UReasoner_DualUtility>();
					
					UState_Map* LumberjackState = NewObject<UState_Map>();
					LumberjackState->SetAsDynamic(true);
					LumberjackState->SetInt("TreesInTheForest", 100);
					LumberjackState->SetInt("Seeds", 500);
					LumberjackState->SetInt("Food", 100);
					LumberjackState->SetInt("Beds", 1);
					LumberjackState->SetFloat("Satiation", 1.f);
					LumberjackState->SetFloat("Rest", 1.f);
					LumberjackState->SetBool("IsCarryingWood", false);
					LumberjackState->SetBool("IsChoppingWood", false);
					LumberjackState->SetBool("IsCarryingSeed", false);
					LumberjackState->SetVector("SeedStoreLocation", FVector::ZeroVector);
					LumberjackState->SetVector("SeedPlantLocation", FVector::ZeroVector);
					LumberjackState->SetVector("TargetFoodOrRest", FVector::ZeroVector);
					LumberjackState->SetVector("NextTreeLocation", FVector::ZeroVector);
					LumberjackState->SetVector("WoodStoreLocation", FVector::ZeroVector);
					LumberjackState->SetVector("CurrentLocation", FVector::ZeroVector);
					LumberjackState->SetAsDynamic(false);
					GivenState = LumberjackState;

					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					// Gather wood goal.
					UReasonablePlanningGoal* GatherWood = NewObject<UReasonablePlanningGoal>();
					GatherWood->SetCategory(1); //Setting a category to a higher value means to only apply this goal if lower value categories are 100% not applicable.

					// Is Applicable: We can only gather wood if there are trees to gather wood from.
					UStateQuery_CompareToInteger* HasTreesInTheForestQuery = NewObject<UStateQuery_CompareToInteger>();
					HasTreesInTheForestQuery->SetQueriedState("TreesInTheForest", EStatePropertyType::Int);
					HasTreesInTheForestQuery->SetComparisonOperation(EStateQueryCompareToOperation::GreaterThan);
					HasTreesInTheForestQuery->SetComparisonValue(0);

					GatherWood->SetIsApplicableQuery(HasTreesInTheForestQuery);

					// Is in Desired State: Our actions have caused us to maximally be near a tree, chop wood, and carry wood, and be near the wood store location.
					// we are done when we are no longer carrying wood, no longer chopping, and are near our wood drop off location. We will also need to add a gate/guard
					// that lets us repeat this action if we start off in a desired state. For instance, immediately after dropping off the wood.
					UStateQuery_CompareToBool* IsNotCarryingWoodQuery = NewObject<UStateQuery_CompareToBool>();
					IsNotCarryingWoodQuery->SetQueriedState("IsCarryingWood", EStatePropertyType::Bool);
					IsNotCarryingWoodQuery->SetComparisonOperation(EStateQueryCompareToOperation::NotEqualTo);
					IsNotCarryingWoodQuery->SetComparisonValue(true);

					UStateQuery_CompareToBool* IsNotChoppingWoodQuery = NewObject<UStateQuery_CompareToBool>();
					IsNotChoppingWoodQuery->SetQueriedState("IsChoppingWood", EStatePropertyType::Bool);
					IsNotChoppingWoodQuery->SetComparisonOperation(EStateQueryCompareToOperation::NotEqualTo);
					IsNotChoppingWoodQuery->SetComparisonValue(true);

					UDistance_State* WoodStoreDistance = NewObject<UDistance_State>();
					WoodStoreDistance->SetLHS("WoodStoreLocation", EStatePropertyType::Vector);
					WoodStoreDistance->SetRHS("CurrentLocation", EStatePropertyType::Vector);

					UStateQuery_CompareDistanceFloat* CloseToWoodStoreQuery = NewObject<UStateQuery_CompareDistanceFloat>();
					CloseToWoodStoreQuery->SetDistance(WoodStoreDistance);
					CloseToWoodStoreQuery->SetComparisonOperation(EStateQueryCompareToOperation::LessThanOrEqualTo);
					CloseToWoodStoreQuery->SetRHS(90000.f);

					UStateQuery_Every* IsInDesiredStateQuery = NewObject<UStateQuery_Every>();
					IsInDesiredStateQuery->SetSubQueries({ IsNotCarryingWoodQuery, IsNotChoppingWoodQuery, CloseToWoodStoreQuery });

					GatherWood->SetIsInDesiredStateQuery(IsInDesiredStateQuery);

					// Distance: Based on whether we have any wood, we need to chop down a tree, and if we are actively chopping, as well as were wood is dropped off
					UDistance_Bool* IsCarryingWoodDistance = NewObject<UDistance_Bool>();
					IsCarryingWoodDistance->SetLHS("IsCarryingWood", EStatePropertyType::Bool);
					IsCarryingWoodDistance->SetRHS(true);

					UDistance_Bool* IsChoppingWoodDistance = NewObject<UDistance_Bool>();
					IsChoppingWoodDistance->SetLHS("IsChoppingWood", EStatePropertyType::Bool);
					IsChoppingWoodDistance->SetRHS(true);

					UDistance_State* DistanceNextTree = NewObject<UDistance_State>();
					DistanceNextTree->SetLHS("CurrentLocation", EStatePropertyType::Vector);
					DistanceNextTree->SetRHS("NextTreeLocation", EStatePropertyType::Vector);

					UDistance_State* DistanceWoodPile = NewObject<UDistance_State>();
					DistanceWoodPile->SetLHS("NextTreeLocation", EStatePropertyType::Vector);
					DistanceWoodPile->SetRHS("WoodStoreLocation", EStatePropertyType::Vector);

					UDistance_AddAll* GatherWoodDistance = NewObject<UDistance_AddAll>();
					GatherWoodDistance->SetSubDistances({ IsCarryingWoodDistance, IsChoppingWoodDistance, DistanceNextTree, DistanceWoodPile });

					GatherWood->SetDistanceCalculator(GatherWoodDistance);

					// Weight: What is the value (utility) of pursuing this goal right now
					UWeight_CurveFloat* GatherWoodWeight = NewObject<UWeight_CurveFloat>();
					GatherWoodWeight->SetWeightStateKeyAndType("TreesInTheForest", EStatePropertyType::Int);

					UCurveFloat* GatherWoodWeightCurve = NewObject<UCurveFloat>();
					GatherWoodWeightCurve->CreateCurveFromCSVString("0,0.0\r\n19,0.1\r\n100,1.0\r\n");
					GatherWoodWeight->SetCurve(GatherWoodWeightCurve);

					GatherWood->SetWeight(GatherWoodWeight);

					// End Gather Wood Goal
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					// Sustain Self Goal
					UReasonablePlanningGoal* SustainSelf = NewObject<UReasonablePlanningGoal>();

					// Is Applicable: Is there food or someplace where rest is possible.
					UStateQuery_CompareToFloat* IsHungry = NewObject<UStateQuery_CompareToFloat>();
					IsHungry->SetQueriedState("Satiation", EStatePropertyType::Float);
					IsHungry->SetComparisonOperation(EStateQueryCompareToOperation::LessThanOrEqualTo);
					IsHungry->SetComparisonValue(0.5f);

					UStateQuery_CompareToFloat* IsTired = NewObject<UStateQuery_CompareToFloat>();
					IsTired->SetQueriedState("Rest", EStatePropertyType::Float);
					IsTired->SetComparisonOperation(EStateQueryCompareToOperation::LessThanOrEqualTo);
					IsTired->SetComparisonValue(0.1f);

					UStateQuery_CompareToInteger* HasFood = NewObject<UStateQuery_CompareToInteger>();
					HasFood->SetQueriedState("Food", EStatePropertyType::Int);
					HasFood->SetComparisonOperation(EStateQueryCompareToOperation::GreaterThan);
					HasFood->SetComparisonValue(0);

					UStateQuery_CompareToInteger* HasBed = NewObject<UStateQuery_CompareToInteger>();
					HasBed->SetQueriedState("Bed", EStatePropertyType::Int);
					HasBed->SetComparisonOperation(EStateQueryCompareToOperation::GreaterThan);
					HasBed->SetComparisonValue(0);

					UStateQuery_Every* IsRestPossibleOrNeeded = NewObject<UStateQuery_Every>();
					IsRestPossibleOrNeeded->SetSubQueries({ IsTired, HasBed });

					UStateQuery_Every* IsSatiationPossibleOrNeeded = NewObject<UStateQuery_Every>();
					IsSatiationPossibleOrNeeded->SetSubQueries({ IsHungry, HasFood });

					UStateQuery_Any* IsApplicable = NewObject<UStateQuery_Any>();
					IsApplicable->SetSubQueries({ IsTired, IsHungry });

					SustainSelf->SetIsApplicableQuery(IsApplicable);

					// Is In Desired State: Satiated (full and well watered) and well rested.
					UStateQuery_CompareToFloat* IsNotHungry = NewObject<UStateQuery_CompareToFloat>();
					IsNotHungry->SetQueriedState("Satiation", EStatePropertyType::Float);
					IsNotHungry->SetComparisonOperation(EStateQueryCompareToOperation::GreaterThan);
					IsNotHungry->SetComparisonValue(0.5f);

					UStateQuery_CompareToFloat* IsNotTired = NewObject<UStateQuery_CompareToFloat>();
					IsNotTired->SetQueriedState("Rest", EStatePropertyType::Float);
					IsNotTired->SetComparisonOperation(EStateQueryCompareToOperation::GreaterThan);
					IsNotTired->SetComparisonValue(0.1f);

					UStateQuery_Every* IsSustainingSelf = NewObject<UStateQuery_Every>();
					IsSustainingSelf->SetSubQueries({ IsNotHungry, IsNotTired });

					SustainSelf->SetIsInDesiredStateQuery(IsSustainingSelf);
					
					// Distance: How much work will it to eat or sleep, such as getting food or getting to a bed
					UDistance_State* DistanceToFoodOrRest = NewObject<UDistance_State>();
					DistanceToFoodOrRest->SetLHS("CurrentLocation", EStatePropertyType::Vector);
					DistanceToFoodOrRest->SetRHS("TargetFoodOrRest", EStatePropertyType::Vector);

					SustainSelf->SetDistanceCalculator(DistanceToFoodOrRest);

					// Weight: The hungrier or more tired the lumberjack is the more weight
					UDistance_Float* FullDistance = NewObject<UDistance_Float>();
					FullDistance->SetLHS("Satiation", EStatePropertyType::Float);
					FullDistance->SetRHS(1.f);

					UDistance_Float* RestDistance = NewObject<UDistance_Float>();
					RestDistance->SetLHS("Rest", EStatePropertyType::Float);
					RestDistance->SetRHS(1.f);

					UDistance_AddAll* RestFullDistance = NewObject<UDistance_AddAll>();
					RestFullDistance->SetSubDistances({ RestDistance, FullDistance });

					UWeight_Distance* SustainWeight = NewObject<UWeight_Distance>();
					SustainWeight->SetDistance(RestFullDistance);

					SustainSelf->SetWeight(SustainWeight);

					// End Sustain Self Goal
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					// Sustain Self Goal
					UReasonablePlanningGoal* PreserveForest = NewObject<UReasonablePlanningGoal>();
					PreserveForest->SetCategory(1);

					// Is Applicable: There are seeds to plant
					UStateQuery_CompareToInteger* HasSeeds = NewObject<UStateQuery_CompareToInteger>();
					HasSeeds->SetQueriedState("Seeds", EStatePropertyType::Int);
					HasSeeds->SetComparisonOperation(EStateQueryCompareToOperation::GreaterThan);
					HasSeeds->SetComparisonValue(0);

					PreserveForest->SetIsApplicableQuery(HasSeeds);

					// Is in Desired State: When we have enough trees planted
					UStateQuery_CompareToInteger* HasEnoughTrees = NewObject<UStateQuery_CompareToInteger>();
					HasEnoughTrees->SetQueriedState("TreesInTheForest", EStatePropertyType::Int);
					HasEnoughTrees->SetComparisonOperation(EStateQueryCompareToOperation::GreaterThanOrEqualTo);
					HasEnoughTrees->SetComparisonValue(20);

					PreserveForest->SetIsInDesiredStateQuery(HasEnoughTrees);

					// Distance: What is the difference in trees needing to be planted
					UDistance_Integer* DistanceFromDesiredMinimumTrees = NewObject<UDistance_Integer>();
					DistanceFromDesiredMinimumTrees->SetLHS("TreesInTheForest", EStatePropertyType::Int);
					DistanceFromDesiredMinimumTrees->SetRHS(20);

					PreserveForest->SetDistanceCalculator(DistanceFromDesiredMinimumTrees);

					//Weight: Distance to having no trees
					// Weight: What is the value (utility) of pursuing this goal right now
					UWeight_CurveFloat* PreserveWeight = NewObject<UWeight_CurveFloat>();
					PreserveWeight->SetWeightStateKeyAndType("TreesInTheForest", EStatePropertyType::Int);

					UCurveFloat* PreserveWeightCurve = NewObject<UCurveFloat>();
					PreserveWeightCurve->CreateCurveFromCSVString("0,1.0\r\n21,0.1\r\n100,0.0\r\n");
					PreserveWeight->SetCurve(PreserveWeightCurve);

					PreserveForest->SetWeight(PreserveWeight);

					// End Sustain Self Goal
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					GivenGoals = {
						GatherWood,
						SustainSelf,
						PreserveForest
					};

					//Every granular action the agent can be tasked to execute, each drives a goal closer to completion
					//We are only testing the planning here so we do not need to assign specific ActionTasks

					//Start: Go To Tree
					UReasonablePlanningAction* GoToTree = NewObject<UReasonablePlanningAction>();

					// Is Applicable: We can only go to a tree a tree if we are not chopping one already, we are not carrying wood, and there are trees in the forest
					UStateQuery_Every* GoToTreeIsApplicable = NewObject<UStateQuery_Every>();
					GoToTreeIsApplicable->SetSubQueries({ HasTreesInTheForestQuery, IsNotChoppingWoodQuery, IsNotCarryingWoodQuery });
					
					GoToTree->SetIsApplicableQuery(GoToTreeIsApplicable);

					// Weight: Distance from the tree, further is a higher weight
					UWeight_Distance* DistanceToTreeWeight = NewObject<UWeight_Distance>();
					DistanceToTreeWeight->SetDistance(DistanceNextTree);
					GoToTree->SetWeightAlgorithm(DistanceToTreeWeight);

					// Mutator: We will be near the tree if we execute this action, for simplicity we are also chopping a tree if we are near it
					UStateMutator_CopyState* CopyNextTreeToCurrentLocation = NewObject<UStateMutator_CopyState>();
					CopyNextTreeToCurrentLocation->SetMutatedStateValue("CurrentLocation", EStatePropertyType::Vector);
					CopyNextTreeToCurrentLocation->SetCopiedFromStateValue("NextTreeLocation", EStatePropertyType::Vector);

					GoToTree->SetStateMutators({ CopyNextTreeToCurrentLocation });

					//End: Go To Tree
					//Start: Chop Tree
					UReasonablePlanningAction* ChopTree = NewObject<UReasonablePlanningAction>();

					// Is Applicable: We can only chop a tree we are near, if we do not have wood already, are not already in the action of chopping, and there is a tree to chop
					UStateQuery_CompareDistanceFloat* IsNearTreeQuery = NewObject<UStateQuery_CompareDistanceFloat>();
					IsNearTreeQuery->SetDistance(DistanceNextTree);
					IsNearTreeQuery->SetComparisonOperation(EStateQueryCompareToOperation::LessThanOrEqualTo);
					IsNearTreeQuery->SetRHS(300.f);

					UStateQuery_Every* ChopTreeIsApplicable = NewObject<UStateQuery_Every>();
					ChopTreeIsApplicable->SetSubQueries({ HasTreesInTheForestQuery, IsNotCarryingWoodQuery, IsNotChoppingWoodQuery, IsNearTreeQuery });

					ChopTree->SetIsApplicableQuery(HasTreesInTheForestQuery);

					// Weight: Whether we are chopping or not
					UDistance_Bool* ChopActionDistance = NewObject<UDistance_Bool>();
					ChopActionDistance->SetLHS("IsChoppingWood", EStatePropertyType::Bool);

					UWeight_Distance* ChopWeight = NewObject<UWeight_Distance>();
					ChopWeight->SetDistance(ChopActionDistance);

					ChopTree->SetWeightAlgorithm(ChopWeight);

					// Mutators: We are now chopping
					UStateMutator_SetValueBool* StartChopping = NewObject<UStateMutator_SetValueBool>();
					StartChopping->SetMutatedStateValue("IsChopppingWood", EStatePropertyType::Bool);
					StartChopping->SetValueToSet(true);

					ChopTree->SetStateMutators({ StartChopping });
					//End: Chop Tree
					//Start: PickUpWood
					UReasonablePlanningAction* PickUpWood = NewObject<UReasonablePlanningAction>();
					PickUpWood->SetIsApplicableQuery(HasTreesInTheForestQuery);

					UReasonablePlanningAction* TakeWoodToPile = NewObject<UReasonablePlanningAction>();
					UReasonablePlanningAction* DropWood = NewObject<UReasonablePlanningAction>();
					UReasonablePlanningAction* GoToFood = NewObject<UReasonablePlanningAction>();
					UReasonablePlanningAction* GoToSeed = NewObject<UReasonablePlanningAction>();
					UReasonablePlanningAction* EatFood = NewObject<UReasonablePlanningAction>();
					UReasonablePlanningAction* GoToPlantingSite = NewObject<UReasonablePlanningAction>();
					UReasonablePlanningAction* GoToBed = NewObject<UReasonablePlanningAction>();
					UReasonablePlanningAction* Sleep = NewObject<UReasonablePlanningAction>();
					UReasonablePlanningAction* PlantSeed = NewObject<UReasonablePlanningAction>();

					GivenActions = {
						GoToTree,
						GoToFood,
						GoToSeed,
						GoToBed,
						ChopTree,
						PickUpWood,
						TakeWoodToPile,
						DropWood,
						EatFood,
						Sleep,
						PlantSeed
					};

				});

			Describe("When given a sleepy state", [this]()
				{
					It("should have a goal to sustain self", [this]()
						{
							GivenState->SetFloat("Rest", 0.05f);

							UReasonablePlanningGoalBase* ActualGoal = GivenReasoner->ReasonNextGoal(GivenGoals, GivenState);
							TestEqual("ReasonNextGoal", ActualGoal, GivenGoals[1]);
						});
				});

			Describe("When given a hungry state", [this]()
				{
					It("should have a goal to sustain self", [this]()
						{
							GivenState->SetFloat("Satiation", 0.1f);

							UReasonablePlanningGoalBase* ActualGoal = GivenReasoner->ReasonNextGoal(GivenGoals, GivenState);
							TestEqual("ReasonNextGoal", ActualGoal, GivenGoals[1]);
						});
				});

			Describe("When given an optimal work state", [this]()
				{
					It("should have a goal to gather wood", [this]()
						{
							int32 PreserveCount = 0;
							int32 HarvestCount = 0;

							for (int32 Idx = 0; Idx < 50; ++Idx)
							{
								GivenState->SetInt("TreesInTheForest", 100 - Idx);
								UReasonablePlanningGoalBase* ActualGoal = GivenReasoner->ReasonNextGoal(GivenGoals, GivenState);
								if (ActualGoal == GivenGoals[2])
								{
									PreserveCount++;
								}
								else if (ActualGoal == GivenGoals[0])
								{
									HarvestCount++;
								}
								else
								{
									AddError("Expected either a preserve forest goal or harvest goal.");
								}
							}

							TestTrue("We are most likely to cut, but it is acceptable to preserve", HarvestCount > PreserveCount);
						});
				});

			Describe("When given a depleted forest state", [this]()
				{
					It("should have a goal to preserve the forest", [this]()
						{
							int32 PreserveCount = 0;
							int32 HarvestCount = 0;

							for (int32 Idx = 0; Idx < 20; ++Idx)
							{
								GivenState->SetInt("TreesInTheForest", Idx);
								UReasonablePlanningGoalBase* ActualGoal = GivenReasoner->ReasonNextGoal(GivenGoals, GivenState);
								if (ActualGoal == GivenGoals[2])
								{
									PreserveCount++;
								}
								else if (ActualGoal == GivenGoals[0])
								{
									HarvestCount++;
								}
								else
								{
									AddError("Expected either a preserve forest goal or harvest goal.");
								}
							}

							TestTrue("We are most likely to preserve, but it is acceptable to cut", PreserveCount > HarvestCount);
						});
				});

			AfterEach([this]()
				{
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