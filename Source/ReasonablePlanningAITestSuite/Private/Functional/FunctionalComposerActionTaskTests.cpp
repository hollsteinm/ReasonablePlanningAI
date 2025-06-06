// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.

#include "Functional/FunctionalComposerActionTaskTests.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "Core/RpaiBrainComponent.h"
#include "Composer/RpaiComposerBehavior.h"
#include "States/RpaiState_Map.h"
#include "Reasoners/RpaiReasoner_AbsoluteUtility.h"
#include "Planners/RpaiPlanner_AStar.h"
#include "Composer/RpaiComposerAction.h"
#include "Composer/RpaiComposerGoal.h"
#include "Composer/Distances/RpaiDistance_AddAll.h"
#include "Composer/Weights/RpaiWeight_ConstantFloat.h"
#include "Composer/Queries/RpaiStateQuery_Every.h"
#include "Composer/Queries/RpaiStateQuery_CompareToBool.h"
#include "Composer/Mutators/RpaiStateMutator_SetValueBool.h"

UWorld* FRpaiAutomationHelpers::GetAutomationWorld()
{
    const TIndirectArray<FWorldContext>& WorldContexts = GEngine->GetWorldContexts();
    for (const FWorldContext& Context : WorldContexts) {
        if (((Context.WorldType == EWorldType::PIE) || (Context.WorldType == EWorldType::Game))
            && (Context.World() != nullptr)) {
            return Context.World();
        }
    }

    return nullptr;
}

AFunctionalTestAutomationAgentController* FRpaiAutomationHelpers::CreateRpaiAgentWithController(UWorld* AutomationWorld, FVector Location)
{
    check(AutomationWorld != nullptr);
    AFunctionalTestAutomationAgentCharacter* Agent = AutomationWorld->SpawnActor<AFunctionalTestAutomationAgentCharacter>(Location, FRotator::ZeroRotator);
    AFunctionalTestAutomationAgentController* AgentController = CastChecked<AFunctionalTestAutomationAgentController>(Agent->GetController());
    return AgentController;
}

void FRpaiAutomationHelpers::CreateSingleActionTaskComposer(URpaiComposerBrainComponent* TargetComposerBrain, URpaiComposerActionTaskBase* AlwaysExecuteAction)
{
    URpaiComposerBehavior* Behavior = NewObject<URpaiComposerBehavior>(TargetComposerBrain->GetOwner());

    URpaiPlannerBase* GivenPlanner = NewObject<URpaiPlanner_AStar>(Behavior);
    URpaiReasonerBase* GivenReasoner = NewObject<URpaiReasoner_AbsoluteUtility>(Behavior);
    URpaiComposerAction* GivenAction = NewObject<URpaiComposerAction>(Behavior);
    URpaiComposerGoal* GivenGoal = NewObject<URpaiComposerGoal>(Behavior);

    URpaiComposerStateQuery* AlwaysTrue = NewObject<URpaiStateQuery_Every>(Behavior);
    URpaiWeight_ConstantFloat* ConstantFloatBestValueForPlanner = NewObject<URpaiWeight_ConstantFloat>(Behavior);
    ConstantFloatBestValueForPlanner->ConstantWeight = TNumericLimits<float>::Min();
    URpaiWeight_ConstantFloat* ConstantFloatBestValueForReasoner = NewObject<URpaiWeight_ConstantFloat>(Behavior);
    ConstantFloatBestValueForPlanner->ConstantWeight = TNumericLimits<float>::Max();
    URpaiComposerDistance* ZeroDistance = NewObject<URpaiDistance_AddAll>(Behavior);

    URpaiStateQuery_CompareToBool* PassAlwaysAfterFirst = NewObject<URpaiStateQuery_CompareToBool>(Behavior);
    PassAlwaysAfterFirst->SetComparisonOperation(EStateQueryCompareToOperation::EqualTo);
    PassAlwaysAfterFirst->SetComparisonValue(true);
    PassAlwaysAfterFirst->SetQueriedState("__SatisfiedAfterOnce", EStatePropertyType::Bool);

    URpaiStateMutator_SetValueBool* SetBooleanValue = NewObject<URpaiStateMutator_SetValueBool>(Behavior);
    SetBooleanValue->SetMutatedStateValue("__SatisfiedAfterOnce", EStatePropertyType::Bool);
    SetBooleanValue->SetValueToSet(true);

    GivenAction->SetIsApplicableQuery(AlwaysTrue);
    GivenAction->SetActionTask(AlwaysExecuteAction);
    GivenAction->SetWeightAlgorithm(ConstantFloatBestValueForPlanner);
    GivenAction->SetStateMutators({ SetBooleanValue });

    GivenGoal->SetCategory(TNumericLimits<int32>::Max());
    GivenGoal->SetDistanceCalculator(ZeroDistance);
    GivenGoal->SetWeight(ConstantFloatBestValueForReasoner);
    GivenGoal->SetIsApplicableQuery(AlwaysTrue);
    GivenGoal->SetIsInDesiredStateQuery(PassAlwaysAfterFirst);

    Behavior->SetConstructedStateType(URpaiState_Map::StaticClass());
    Behavior->SetPlanner(GivenPlanner);
    Behavior->SetReasoner(GivenReasoner);
    Behavior->SetActions({ GivenAction });
    Behavior->SetGoals({ GivenGoal });

    TargetComposerBrain->SetReasonablePlanningBehavior(Behavior);
    TargetComposerBrain->RestartLogic();
}

AFunctionalTestAutomationAgentController::AFunctionalTestAutomationAgentController()
{
    RpaiBrainComponent = CreateDefaultSubobject<URpaiComposerBrainComponent>(TEXT("TestAutomationBrain"));
    BrainComponent = RpaiBrainComponent;
}

AFunctionalTestAutomationAgentCharacter::AFunctionalTestAutomationAgentCharacter()
{
    AIControllerClass = AFunctionalTestAutomationAgentController::StaticClass();
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}