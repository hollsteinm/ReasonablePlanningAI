// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "ReasonablePlanningAITestTypes.h"
#include "States/RpaiState_Map.h"
#include "Planners/RpaiPlanner_AStar.h"
#include "Reasoners/RpaiReasoner_DualUtility.h"
#include "Reasoners/RpaiReasoner_AbsoluteUtility.h"
#include "Composer/RpaiComposerAction.h"
#include "Composer/RpaiComposerGoal.h"
#include "Composer/Distances/RpaiDistance_State.h"
#include "Composer/Distances/RpaiDistance_Bool.h"
#include "Composer/Distances/RpaiDistance_Float.h"
#include "Composer/Distances/RpaiDistance_Integer.h"
#include "Composer/Distances/RpaiDistance_Rotator.h"
#include "Composer/Distances/RpaiDistance_Vector.h"
#include "Composer/Distances/RpaiDistance_AddAll.h"
#include "Composer/Distances/RpaiDistance_Select.h"
#include "Composer/Mutators/RpaiStateMutator_AddFloat.h"
#include "Composer/Mutators/RpaiStateMutator_AddInteger.h"
#include "Composer/Mutators/RpaiStateMutator_MultiplyFloat.h"
#include "Composer/Mutators/RpaiStateMutator_MultiplyInteger.h"
#include "Composer/Mutators/RpaiStateMutator_SetValueBool.h"
#include "Composer/Mutators/RpaiStateMutator_SetValueClass.h"
#include "Composer/Mutators/RpaiStateMutator_SetValueFloat.h"
#include "Composer/Mutators/RpaiStateMutator_SetValueInteger.h"
#include "Composer/Mutators/RpaiStateMutator_SetValueName.h"
#include "Composer/Mutators/RpaiStateMutator_SetValueRotator.h"
#include "Composer/Mutators/RpaiStateMutator_SetValueString.h"
#include "Composer/Mutators/RpaiStateMutator_SetValueVector.h"
#include "Composer/Mutators/RpaiStateMutator_CopyState.h"
#include "Composer/Queries/RpaiStateQuery_CompareDistanceFloat.h"
#include "Composer/Queries/RpaiStateQuery_CompareToBool.h"
#include "Composer/Queries/RpaiStateQuery_CompareToDistance.h"
#include "Composer/Queries/RpaiStateQuery_CompareToFloat.h"
#include "Composer/Queries/RpaiStateQuery_CompareToInteger.h"
#include "Composer/Queries/RpaiStateQuery_CompareToStateValue.h"
#include "Composer/Queries/RpaiStateQuery_Every.h"
#include "Composer/Queries/RpaiStateQuery_Any.h"
#include "Composer/Weights/RpaiWeight_Float.h"
#include "Composer/Weights/RpaiWeight_CurveFloat.h"
#include "Composer/Weights/RpaiWeight_ConstantFloat.h"
#include "Composer/Weights/RpaiWeight_AddAll.h"
#include "Composer/Weights/RpaiWeight_Distance.h"
#include "Composer/Weights/RpaiWeight_Select.h"

BEGIN_DEFINE_SPEC(ReasonablePlanningComposerSpec, "ReasonablePlanningAI.Composer", EAutomationTestFlags::ProductFilter | EAutomationTestFlags_ApplicationContextMask)
   URpaiPlannerBase* GivenPlanner;
   URpaiReasonerBase* GivenReasoner;
   URpaiState* GivenState;
   TArray<URpaiGoalBase*> GivenGoals;
   TArray<URpaiActionBase*> GivenActions;
END_DEFINE_SPEC(ReasonablePlanningComposerSpec)
void ReasonablePlanningComposerSpec::Define()
{
   Describe("A Lumberjack", [this]()
      {
         BeforeEach([this]()
            {
               GivenPlanner = NewObject<URpaiPlanner_AStar>();
               GivenReasoner = NewObject<URpaiReasoner_DualUtility>();
               
               URpaiState_Map* LumberjackState = NewObject<URpaiState_Map>();
               LumberjackState->SetAsDynamic(true);
               LumberjackState->SetInt("TreesInTheForest", 100);
               LumberjackState->SetInt("LogsLeftToCarry", 0);
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
               LumberjackState->SetVector("NextLogLocation", FVector::ZeroVector);
               LumberjackState->SetVector("CurrentLocation", FVector::ZeroVector);
               LumberjackState->SetAsDynamic(false);
               GivenState = LumberjackState;

               /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
               // Gather wood goal.
               URpaiComposerGoal* GatherWood = NewObject<URpaiComposerGoal>();
               GatherWood->SetCategory(1); //Setting a category to a higher value means to only apply this goal if lower value categories are 100% not applicable.

               // Is Applicable: We can only gather wood if there are trees to gather wood from.
               URpaiStateQuery_CompareToInteger* HasTreesInTheForestQuery = NewObject<URpaiStateQuery_CompareToInteger>();
               HasTreesInTheForestQuery->SetQueriedState("TreesInTheForest", EStatePropertyType::Int);
               HasTreesInTheForestQuery->SetComparisonOperation(EStateQueryCompareToOperation::GreaterThan);
               HasTreesInTheForestQuery->SetComparisonValue(0);

               GatherWood->SetIsApplicableQuery(HasTreesInTheForestQuery);

               // Is in Desired State: Our actions have caused us to maximally be near a tree, chop wood, and carry wood, and be near the wood store location.
               // we are done when we are no longer carrying wood, no longer chopping, and are near our wood drop off location. We will also need to add a gate/guard
               // that lets us repeat this action if we start off in a desired state. For instance, immediately after dropping off the wood.
               URpaiStateQuery_CompareToBool* IsNotCarryingWoodQuery = NewObject<URpaiStateQuery_CompareToBool>();
               IsNotCarryingWoodQuery->SetQueriedState("IsCarryingWood", EStatePropertyType::Bool);
               IsNotCarryingWoodQuery->SetComparisonOperation(EStateQueryCompareToOperation::NotEqualTo);
               IsNotCarryingWoodQuery->SetComparisonValue(true);

               URpaiStateQuery_CompareToBool* IsNotChoppingWoodQuery = NewObject<URpaiStateQuery_CompareToBool>();
               IsNotChoppingWoodQuery->SetQueriedState("IsChoppingWood", EStatePropertyType::Bool);
               IsNotChoppingWoodQuery->SetComparisonOperation(EStateQueryCompareToOperation::NotEqualTo);
               IsNotChoppingWoodQuery->SetComparisonValue(true);

               URpaiDistance_State* WoodStoreDistance = NewObject<URpaiDistance_State>();
               WoodStoreDistance->SetLHS("WoodStoreLocation", EStatePropertyType::Vector);
               WoodStoreDistance->SetRHS("CurrentLocation", EStatePropertyType::Vector);

               URpaiStateQuery_CompareDistanceFloat* CloseToWoodStoreQuery = NewObject<URpaiStateQuery_CompareDistanceFloat>();
               CloseToWoodStoreQuery->SetDistance(WoodStoreDistance);
               CloseToWoodStoreQuery->SetComparisonOperation(EStateQueryCompareToOperation::LessThanOrEqualTo);
               CloseToWoodStoreQuery->SetRHS(300.f);

               URpaiStateQuery_Every* IsInDesiredStateQuery = NewObject<URpaiStateQuery_Every>();
               IsInDesiredStateQuery->SetSubQueries({ IsNotCarryingWoodQuery, IsNotChoppingWoodQuery, CloseToWoodStoreQuery });

               GatherWood->SetIsInDesiredStateQuery(IsInDesiredStateQuery);

               // Distance: Based on whether we have any wood, we need to chop down a tree, and if we are actively chopping, as well as were wood is dropped off
               // This is complex distance requiring URpaiDistance_Select. We do not care about the distance to the next tree to cut if we are carrying wood, we only
               // care about the distance to the wood store location if we are carrying wood, so on and so forth
               URpaiDistance_Bool* IsCarryingWoodDistance = NewObject<URpaiDistance_Bool>();
               IsCarryingWoodDistance->SetLHS("IsCarryingWood", EStatePropertyType::Bool);
               IsCarryingWoodDistance->SetRHS(true);

               URpaiStateQuery_CompareToBool* IsCarryingWoodQuery = NewObject<URpaiStateQuery_CompareToBool>();
               IsCarryingWoodQuery->SetQueriedState("IsCarryingWood", EStatePropertyType::Bool);
               IsCarryingWoodQuery->SetComparisonOperation(EStateQueryCompareToOperation::EqualTo);
               IsCarryingWoodQuery->SetComparisonValue(true);

               URpaiDistance_Bool* IsChoppingWoodDistance = NewObject<URpaiDistance_Bool>();
               IsChoppingWoodDistance->SetLHS("IsChoppingWood", EStatePropertyType::Bool);
               IsChoppingWoodDistance->SetRHS(true);

               URpaiDistance_State* DistanceNextTree = NewObject<URpaiDistance_State>();
               DistanceNextTree->SetLHS("CurrentLocation", EStatePropertyType::Vector);
               DistanceNextTree->SetRHS("NextTreeLocation", EStatePropertyType::Vector);

               URpaiDistance_State* DistanceWoodPile = NewObject<URpaiDistance_State>();
               DistanceWoodPile->SetLHS("CurrentLocation", EStatePropertyType::Vector);
               DistanceWoodPile->SetRHS("WoodStoreLocation", EStatePropertyType::Vector);

               FDistanceSelectStateQueryPair AlreadyCarryingWood;
               AlreadyCarryingWood.SelectionQuery = IsCarryingWoodQuery;
               AlreadyCarryingWood.SelectionDistance = DistanceWoodPile;

               URpaiDistance_Select* GatherWoodDistance = NewObject<URpaiDistance_Select>();
               GatherWoodDistance->SetSelections({
                  AlreadyCarryingWood,
               });

               URpaiDistance_AddAll* GatherWoodDistanceDefault = NewObject<URpaiDistance_AddAll>();
               GatherWoodDistanceDefault->SetSubDistances({ IsChoppingWoodDistance, DistanceNextTree });

               GatherWoodDistance->SetDefault(DistanceNextTree);

               GatherWood->SetDistanceCalculator(GatherWoodDistance);

               // Weight: What is the value (utility) of pursuing this goal right now
               URpaiWeight_CurveFloat* GatherWoodWeight = NewObject<URpaiWeight_CurveFloat>();
               GatherWoodWeight->SetWeightStateKeyAndType("TreesInTheForest", EStatePropertyType::Int);

               UCurveFloat* GatherWoodWeightCurve = NewObject<UCurveFloat>();
               GatherWoodWeightCurve->CreateCurveFromCSVString("0,0.0\r\n19,0.1\r\n100,1.0\r\n");
               GatherWoodWeight->SetCurve(GatherWoodWeightCurve);

               GatherWood->SetWeight(GatherWoodWeight);

               // End Gather Wood Goal
               /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
               
               /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
               // Sustain Self Goal
               URpaiComposerGoal* SustainSelf = NewObject<URpaiComposerGoal>();

               // Is Applicable: Is there food or someplace where rest is possible.
               URpaiStateQuery_CompareToFloat* IsHungry = NewObject<URpaiStateQuery_CompareToFloat>();
               IsHungry->SetQueriedState("Satiation", EStatePropertyType::Float);
               IsHungry->SetComparisonOperation(EStateQueryCompareToOperation::LessThanOrEqualTo);
               IsHungry->SetComparisonValue(0.5f);

               URpaiStateQuery_CompareToFloat* IsTired = NewObject<URpaiStateQuery_CompareToFloat>();
               IsTired->SetQueriedState("Rest", EStatePropertyType::Float);
               IsTired->SetComparisonOperation(EStateQueryCompareToOperation::LessThanOrEqualTo);
               IsTired->SetComparisonValue(0.1f);

               URpaiStateQuery_CompareToInteger* HasFood = NewObject<URpaiStateQuery_CompareToInteger>();
               HasFood->SetQueriedState("Food", EStatePropertyType::Int);
               HasFood->SetComparisonOperation(EStateQueryCompareToOperation::GreaterThan);
               HasFood->SetComparisonValue(0);

               URpaiStateQuery_CompareToInteger* HasBed = NewObject<URpaiStateQuery_CompareToInteger>();
               HasBed->SetQueriedState("Bed", EStatePropertyType::Int);
               HasBed->SetComparisonOperation(EStateQueryCompareToOperation::GreaterThan);
               HasBed->SetComparisonValue(0);

               URpaiStateQuery_Every* IsRestPossibleOrNeeded = NewObject<URpaiStateQuery_Every>();
               IsRestPossibleOrNeeded->SetSubQueries({ IsTired, HasBed });

               URpaiStateQuery_Every* IsSatiationPossibleOrNeeded = NewObject<URpaiStateQuery_Every>();
               IsSatiationPossibleOrNeeded->SetSubQueries({ IsHungry, HasFood });

               URpaiStateQuery_Any* IsApplicable = NewObject<URpaiStateQuery_Any>();
               IsApplicable->SetSubQueries({ IsTired, IsHungry });

               SustainSelf->SetIsApplicableQuery(IsApplicable);

               // Is In Desired State: Satiated (full and well watered) and well rested.
               URpaiStateQuery_CompareToFloat* IsNotHungry = NewObject<URpaiStateQuery_CompareToFloat>();
               IsNotHungry->SetQueriedState("Satiation", EStatePropertyType::Float);
               IsNotHungry->SetComparisonOperation(EStateQueryCompareToOperation::GreaterThan);
               IsNotHungry->SetComparisonValue(0.5f);

               URpaiStateQuery_CompareToFloat* IsNotTired = NewObject<URpaiStateQuery_CompareToFloat>();
               IsNotTired->SetQueriedState("Rest", EStatePropertyType::Float);
               IsNotTired->SetComparisonOperation(EStateQueryCompareToOperation::GreaterThan);
               IsNotTired->SetComparisonValue(0.1f);

               URpaiStateQuery_Every* IsSustainingSelf = NewObject<URpaiStateQuery_Every>();
               IsSustainingSelf->SetSubQueries({ IsNotHungry, IsNotTired });

               SustainSelf->SetIsInDesiredStateQuery(IsSustainingSelf);
               
               // Distance: How much work will it to eat or sleep, such as getting food or getting to a bed
               URpaiDistance_State* DistanceToFoodOrRest = NewObject<URpaiDistance_State>();
               DistanceToFoodOrRest->SetLHS("CurrentLocation", EStatePropertyType::Vector);
               DistanceToFoodOrRest->SetRHS("TargetFoodOrRest", EStatePropertyType::Vector);

               SustainSelf->SetDistanceCalculator(DistanceToFoodOrRest);

               // Weight: The hungrier or more tired the lumberjack is the more weight
               URpaiDistance_Float* FullDistance = NewObject<URpaiDistance_Float>();
               FullDistance->SetLHS("Satiation", EStatePropertyType::Float);
               FullDistance->SetRHS(1.f);

               URpaiDistance_Float* RestDistance = NewObject<URpaiDistance_Float>();
               RestDistance->SetLHS("Rest", EStatePropertyType::Float);
               RestDistance->SetRHS(1.f);

               URpaiDistance_AddAll* RestFullDistance = NewObject<URpaiDistance_AddAll>();
               RestFullDistance->SetSubDistances({ RestDistance, FullDistance });

               URpaiWeight_Distance* SustainWeight = NewObject<URpaiWeight_Distance>();
               SustainWeight->SetDistance(RestFullDistance);

               SustainSelf->SetWeight(SustainWeight);

               // End Sustain Self Goal
               /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

               /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
               // Sustain Self Goal
               URpaiComposerGoal* PreserveForest = NewObject<URpaiComposerGoal>();
               PreserveForest->SetCategory(1);

               // Is Applicable: There are seeds to plant
               URpaiStateQuery_CompareToInteger* HasSeeds = NewObject<URpaiStateQuery_CompareToInteger>();
               HasSeeds->SetQueriedState("Seeds", EStatePropertyType::Int);
               HasSeeds->SetComparisonOperation(EStateQueryCompareToOperation::GreaterThan);
               HasSeeds->SetComparisonValue(0);

               PreserveForest->SetIsApplicableQuery(HasSeeds);

               // Is in Desired State: When we have enough trees planted
               URpaiStateQuery_CompareToInteger* HasEnoughTrees = NewObject<URpaiStateQuery_CompareToInteger>();
               HasEnoughTrees->SetQueriedState("TreesInTheForest", EStatePropertyType::Int);
               HasEnoughTrees->SetComparisonOperation(EStateQueryCompareToOperation::GreaterThanOrEqualTo);
               HasEnoughTrees->SetComparisonValue(20);

               PreserveForest->SetIsInDesiredStateQuery(HasEnoughTrees);

               // Distance: What is the difference in trees needing to be planted
               URpaiDistance_Integer* DistanceFromDesiredMinimumTrees = NewObject<URpaiDistance_Integer>();
               DistanceFromDesiredMinimumTrees->SetLHS("TreesInTheForest", EStatePropertyType::Int);
               DistanceFromDesiredMinimumTrees->SetRHS(20);

               PreserveForest->SetDistanceCalculator(DistanceFromDesiredMinimumTrees);

               //Weight: Distance to having no trees
               // Weight: What is the value (utility) of pursuing this goal right now
               URpaiWeight_CurveFloat* PreserveWeight = NewObject<URpaiWeight_CurveFloat>();
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

               /////////////////////////////////////////////////////// ACTIONS //////////////////////////////////////////////////////////////////////////////////////

               //Every granular action the agent can be tasked to execute, each drives a goal closer to completion
               //We are only testing the planning here so we do not need to assign specific ActionTasks

               //Start: Go To Tree
               URpaiComposerAction* GoToTree = NewObject<URpaiComposerAction>();

               // Is Applicable: We can only go to a tree a tree if we are not chopping one already, we are not carrying wood, and there are trees in the forest
               // Using the query of NoLogsToCarry to force all logs to be picked up before cutting more trees
               URpaiStateQuery_CompareToInteger* NoLogsToCarry = NewObject<URpaiStateQuery_CompareToInteger>();
               NoLogsToCarry->SetQueriedState("LogsLeftToCarry", EStatePropertyType::Int);
               NoLogsToCarry->SetComparisonOperation(EStateQueryCompareToOperation::LessThanOrEqualTo);
               NoLogsToCarry->SetComparisonValue(0);

               URpaiStateQuery_Every* GoToTreeIsApplicable = NewObject<URpaiStateQuery_Every>();
               GoToTreeIsApplicable->SetSubQueries({ HasTreesInTheForestQuery, IsNotChoppingWoodQuery, IsNotCarryingWoodQuery, NoLogsToCarry });
               
               GoToTree->SetIsApplicableQuery(GoToTreeIsApplicable);

               // Weight: Distance from the tree, further is a higher weight
               URpaiWeight_Distance* DistanceToTreeWeight = NewObject<URpaiWeight_Distance>();
               DistanceToTreeWeight->SetDistance(DistanceNextTree);
               GoToTree->SetWeightAlgorithm(DistanceToTreeWeight);

               // Mutator: We will be near the tree if we execute this action, for simplicity we are also chopping a tree if we are near it
               URpaiStateMutator_CopyState* CopyNextTreeToCurrentLocation = NewObject<URpaiStateMutator_CopyState>();
               CopyNextTreeToCurrentLocation->SetMutatedStateValue("CurrentLocation", EStatePropertyType::Vector);
               CopyNextTreeToCurrentLocation->SetCopiedFromStateValue("NextTreeLocation", EStatePropertyType::Vector);

               URpaiStateMutator_SetValueBool* StartChopping = NewObject<URpaiStateMutator_SetValueBool>();
               StartChopping->SetMutatedStateValue("IsChoppingWood", EStatePropertyType::Bool);
               StartChopping->SetValueToSet(true);

               GoToTree->SetStateMutators({ CopyNextTreeToCurrentLocation, StartChopping });

               //End: Go To Tree
               //Start: Chop Tree
               URpaiComposerAction* ChopTree = NewObject<URpaiComposerAction>();

               // Is Applicable: Two conditions make this applicable: agent is near a tree in a forest that has trees, or agent is already chopping.
               URpaiStateQuery_CompareToBool* IsAlreadyChoppingWoodQuery = NewObject<URpaiStateQuery_CompareToBool>();
               IsAlreadyChoppingWoodQuery->SetQueriedState("IsChoppingWood", EStatePropertyType::Bool);
               IsAlreadyChoppingWoodQuery->SetComparisonOperation(EStateQueryCompareToOperation::EqualTo);
               IsAlreadyChoppingWoodQuery->SetComparisonValue(true);

               URpaiStateQuery_CompareDistanceFloat* CloseToTreeQuery = NewObject<URpaiStateQuery_CompareDistanceFloat>();
               CloseToTreeQuery->SetDistance(DistanceNextTree);
               CloseToTreeQuery->SetComparisonOperation(EStateQueryCompareToOperation::LessThanOrEqualTo);
               CloseToTreeQuery->SetRHS(300.f);

               URpaiStateQuery_Every* ChopTreeIsApplicableTreesInForest = NewObject<URpaiStateQuery_Every>();
               ChopTreeIsApplicableTreesInForest->SetSubQueries({ IsAlreadyChoppingWoodQuery, NoLogsToCarry, HasTreesInTheForestQuery });

               URpaiStateQuery_Every* ChopTreeIsApplicableChopping = NewObject<URpaiStateQuery_Every>();
               ChopTreeIsApplicableChopping->SetSubQueries({ CloseToTreeQuery, NoLogsToCarry, HasTreesInTheForestQuery });

               URpaiStateQuery_Any* ChopTreeIsApplicable = NewObject<URpaiStateQuery_Any>();
               ChopTreeIsApplicable->SetSubQueries({ ChopTreeIsApplicableTreesInForest, ChopTreeIsApplicableChopping });

               ChopTree->SetIsApplicableQuery(ChopTreeIsApplicable);

               // Weight: Whether we are chopping or not, how far from even chopping
               URpaiDistance_Bool* ChopActionDistance = NewObject<URpaiDistance_Bool>();
               ChopActionDistance->SetLHS("IsChoppingWood", EStatePropertyType::Bool);
               ChopActionDistance->SetRHS(true);

               URpaiWeight_Distance* ChopActionWeight = NewObject<URpaiWeight_Distance>();
               ChopActionWeight->SetDistance(ChopActionDistance);

               URpaiWeight_AddAll* ChopWeight = NewObject<URpaiWeight_AddAll>();
               ChopWeight->SetSubWeights({ DistanceToTreeWeight, ChopActionWeight });

               ChopTree->SetWeightAlgorithm(ChopWeight);

               // Mutators: Mutators should try to only reflect state after the action is completed.
               URpaiStateMutator_CopyState* ChoppedWoodLocation = NewObject<URpaiStateMutator_CopyState>();
               ChoppedWoodLocation->SetMutatedStateValue("NextLogLocation", EStatePropertyType::Vector);
               ChoppedWoodLocation->SetCopiedFromStateValue("CurrentLocation", EStatePropertyType::Vector);

               URpaiStateMutator_AddInteger* AddLogsToCarry = NewObject<URpaiStateMutator_AddInteger>();
               AddLogsToCarry->SetMutatedStateValue("LogsLeftToCarry", EStatePropertyType::Int);
               AddLogsToCarry->SetIntegerValueToAdd(1);

               URpaiStateMutator_AddInteger* RemoveTreeFromForest = NewObject<URpaiStateMutator_AddInteger>();
               RemoveTreeFromForest->SetMutatedStateValue("TreesInTheForest", EStatePropertyType::Int);
               RemoveTreeFromForest->SetIntegerValueToAdd(-100); //For test purposes we will chop down all trees in this one iteration, this is so we can fudge it and avoid goal selection of planting seeds

               URpaiStateMutator_SetValueBool* NoLongerChoppingWood = NewObject<URpaiStateMutator_SetValueBool>();
               NoLongerChoppingWood->SetMutatedStateValue("IsChoppingWood", EStatePropertyType::Bool);
               NoLongerChoppingWood->SetValueToSet(false);

               ChopTree->SetStateMutators({ AddLogsToCarry, ChoppedWoodLocation, RemoveTreeFromForest, NoLongerChoppingWood });
               //End: Chop Tree
               //Start: GoToLogToCarry
               URpaiComposerAction* GoToLogToCarry = NewObject<URpaiComposerAction>();
               
               // Is Applicable: are there logs to carry
               URpaiStateQuery_CompareToInteger* HasLogsToCarryQuery = NewObject<URpaiStateQuery_CompareToInteger>();
               HasLogsToCarryQuery->SetQueriedState("LogsLeftToCarry", EStatePropertyType::Int);
               HasLogsToCarryQuery->SetComparisonOperation(EStateQueryCompareToOperation::GreaterThan);
               HasLogsToCarryQuery->SetComparisonValue(0);

               GoToLogToCarry->SetIsApplicableQuery(HasLogsToCarryQuery);

               // Weight: How far to the log
               URpaiDistance_State* DistanceToNextLog = NewObject<URpaiDistance_State>();
               DistanceToNextLog->SetLHS("CurrentLocation", EStatePropertyType::Vector);
               DistanceToNextLog->SetRHS("NextLogLocation", EStatePropertyType::Vector);

               URpaiWeight_Distance* GoToLogToCarryWeight = NewObject<URpaiWeight_Distance>();
               GoToLogToCarryWeight->SetDistance(DistanceToNextLog);

               GoToLogToCarry->SetWeightAlgorithm(GoToLogToCarryWeight);

               // Mutators: agent is now carrying a log
               URpaiStateMutator_CopyState* AtLogMutator = NewObject<URpaiStateMutator_CopyState>();
               AtLogMutator->SetMutatedStateValue("CurrentLocation", EStatePropertyType::Vector);
               AtLogMutator->SetCopiedFromStateValue("NextLogLocation", EStatePropertyType::Vector);

               URpaiStateMutator_SetValueBool* CarryingLogMutator = NewObject<URpaiStateMutator_SetValueBool>();
               CarryingLogMutator->SetMutatedStateValue("IsCarryingWood", EStatePropertyType::Bool);
               CarryingLogMutator->SetValueToSet(true);

               URpaiStateMutator_AddInteger* OneLessLogToCarry = NewObject<URpaiStateMutator_AddInteger>();
               OneLessLogToCarry->SetMutatedStateValue("LogsLeftToCarry", EStatePropertyType::Int);
               OneLessLogToCarry->SetIntegerValueToAdd(-1);

               GoToLogToCarry->SetStateMutators({ AtLogMutator, CarryingLogMutator, OneLessLogToCarry });

               //End: GoToLogToCarry
               //Start: TakeWoodToPile
               URpaiComposerAction* TakeWoodToPile = NewObject<URpaiComposerAction>();

               // IsApplicable: We are carrying wood
               TakeWoodToPile->SetIsApplicableQuery(IsCarryingWoodQuery);

               // Weight: Distance to the wood pile
               URpaiWeight_Distance* TakeWoodToPileWeight = NewObject<URpaiWeight_Distance>();
               TakeWoodToPileWeight->SetDistance(DistanceWoodPile);

               TakeWoodToPile->SetWeightAlgorithm(TakeWoodToPileWeight);

               // Mutator: no longer carrying wood and our location is now at the wood pile
               URpaiStateMutator_CopyState* AtWoodPileMutator = NewObject<URpaiStateMutator_CopyState>();
               AtWoodPileMutator->SetMutatedStateValue("CurrentLocation", EStatePropertyType::Vector);
               AtWoodPileMutator->SetCopiedFromStateValue("WoodStoreLocation", EStatePropertyType::Vector);

               URpaiStateMutator_SetValueBool* NotCarryingWoodMutator = NewObject<URpaiStateMutator_SetValueBool>();
               NotCarryingWoodMutator->SetMutatedStateValue("IsCarryingWood", EStatePropertyType::Bool);
               NotCarryingWoodMutator->SetValueToSet(false);

               TakeWoodToPile->SetStateMutators({ AtWoodPileMutator, NotCarryingWoodMutator });
               //End: TakeWoodToPile

               GivenActions = {
                  GoToTree,
                  ChopTree,
                  GoToLogToCarry,
                  TakeWoodToPile,
               };

            });

         Describe("When given a sleepy state", [this]()
            {
               It("should have a goal to sustain self", [this]()
                  {
                     GivenState->SetFloat("Rest", 0.05f);

                     URpaiGoalBase* ActualGoal = GivenReasoner->ReasonNextGoal(GivenGoals, GivenState);
                     TestEqual("ReasonNextGoal", ActualGoal, GivenGoals[1]);
                  });
            });

         Describe("When given a hungry state", [this]()
            {
               It("should have a goal to sustain self", [this]()
                  {
                     GivenState->SetFloat("Satiation", 0.1f);

                     URpaiGoalBase* ActualGoal = GivenReasoner->ReasonNextGoal(GivenGoals, GivenState);
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
                        URpaiGoalBase* ActualGoal = GivenReasoner->ReasonNextGoal(GivenGoals, GivenState);
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
                        URpaiGoalBase* ActualGoal = GivenReasoner->ReasonNextGoal(GivenGoals, GivenState);
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

         Describe("planning actions", [this]()
            {
               Describe("chopping wood goal", [this]
                  {
                     It("should have a plan to go to a tree, chop it, grab the wood, take it to the pile", [this]()
                        {
                           //Put us away from the desired end state
                           GivenState->SetValueOfType("WoodStoreLocation", FVector(9000.0f, 90000.f, 10000.f));
                           GivenState->SetValueOfType("CurrentLocation", FVector(-9000.0f, -90000.f, -10000.f));

                           auto ActualGoal = GivenReasoner->ReasonNextGoal(GivenGoals, GivenState);
                           TestEqual("Selected Goal", ActualGoal, GivenGoals[0]);

                           TArray<URpaiActionBase*> ActualActions;
                           bool bSuccess = GivenPlanner->PlanChosenGoal(ActualGoal, GivenState, GivenActions, ActualActions);

                           if (TestTrue("Success", bSuccess) && TestEqual("Number of Actions", ActualActions.Num(), 4))
                           {

                              // Output plan is meant to be popped. Thus invert the expected order.
                              TestEqual("Go To Tree", ActualActions[3], GivenActions[0]);
                              TestEqual("Chop Wood", ActualActions[2], GivenActions[1]);
                              TestEqual("Go To Wood Pile", ActualActions[1], GivenActions[2]);
                              TestEqual("Drop Off Wood", ActualActions[0], GivenActions[3]);
                           }
                        });
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