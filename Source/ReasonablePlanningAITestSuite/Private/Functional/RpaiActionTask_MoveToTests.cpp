// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Tests/AutomationEditorCommon.h"
#include "Tests/AutomationCommon.h"
#include "Editor.h"
#include "Kismet/GameplayStatics.h"
#include "Functional/FunctionalComposerActionTaskTests.h"
#include "Composer/ActionTasks/RpaiActionTask_MoveTo.h"
#include "States/RpaiState_Map.h"

#if WITH_DEV_AUTOMATION_TESTS

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FWaitToArriveAtDestinationLatentCommand, FVector, Destination);
bool FWaitToArriveAtDestinationLatentCommand::Update()
{
    UWorld* AutoWorld = FRpaiAutomationHelpers::GetAutomationWorld();
    if (AutoWorld == nullptr)
    {
        return false;
    }

    APawn* Agent = CastChecked<AFunctionalTestAutomationAgentCharacter>(UGameplayStatics::GetActorOfClass(AutoWorld, AFunctionalTestAutomationAgentCharacter::StaticClass()));
    if (Agent == nullptr)
    {
        return false;
    }

    const auto AgentLocation = Agent->GetActorLocation();
    return FVector::DistSquared(AgentLocation, Destination) <= 1600;
}

DEFINE_LATENT_AUTOMATION_COMMAND(FWaitForPieWorld);
bool FWaitForPieWorld::Update()
{
    check(GEditor != nullptr);
    return GEditor->IsPlayingSessionInEditor();
}

DEFINE_LATENT_AUTOMATION_COMMAND(FSpawnTestAgent);
bool FSpawnTestAgent::Update()
{
    UWorld* AutoWorld = FRpaiAutomationHelpers::GetAutomationWorld();
    if (AutoWorld == nullptr)
    {
        return false;
    }

    AFunctionalTestAutomationAgentController* AgentController = FRpaiAutomationHelpers::CreateRpaiAgentWithController(AutoWorld, FVector(0.f, 0.f, 90.f));
    if (AgentController == nullptr)
    {
        return false;
    }

    return true;
}

DEFINE_LATENT_AUTOMATION_COMMAND(FExecuteSimpleMoveToActionTaskLatentCommand);
bool FExecuteSimpleMoveToActionTaskLatentCommand::Update()
{
    UWorld* AutoWorld = FRpaiAutomationHelpers::GetAutomationWorld();
    if (AutoWorld == nullptr)
    {
        return false;
    }

    APawn* Agent = CastChecked<AFunctionalTestAutomationAgentCharacter>(UGameplayStatics::GetActorOfClass(AutoWorld, AFunctionalTestAutomationAgentCharacter::StaticClass()));
    if (Agent == nullptr)
    {
        return false;
    }

    AFunctionalTestAutomationAgentController* AgentController = CastChecked<AFunctionalTestAutomationAgentController>(Agent->GetController());
    if (AgentController == nullptr)
    {
        return false;
    }

    FVector MoveToDestination = FVector(200.f, 0.0f, 40.0f);
    URpaiActionTask_MoveTo* MoveToTask = NewObject<URpaiActionTask_MoveTo>(AgentController);

    FStateKeyValueReference MoveToReference;
    MoveToReference.ExpectedValueType = EStatePropertyType::Vector;
    MoveToReference.StateKeyName = "MoveTo";
    MoveToTask->SetStateKeyValueReference(MoveToReference);

    FRpaiAutomationHelpers::CreateSingleActionTaskComposer(AgentController->GetRpaiBrainComponent(), MoveToTask);
    URpaiState_Map* StateMap = CastChecked<URpaiState_Map>(AgentController->GetRpaiBrainComponent()->LoadOrCreateStateFromAi());
    StateMap->SetAsDynamic(true);
    StateMap->SetBool("__SatisfiedAfterOnce", false);
    StateMap->SetVector("MoveTo", MoveToDestination);
    StateMap->SetAsDynamic(false);

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRpaiActionTask_MoveTo_PawnShouldMove, "ReasonablePlanningAI.Composer.Functional.ActionTask_MoveTo", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter)
bool FRpaiActionTask_MoveTo_PawnShouldMove::RunTest(const FString& Parameters)
{
    ADD_LATENT_AUTOMATION_COMMAND(FEditorLoadMap("/ReasonablePlanningAI/RpaiAutomationTestWorld"));
    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(2.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FStartPIECommand(true));
    ADD_LATENT_AUTOMATION_COMMAND(FWaitForPieWorld());
    ADD_LATENT_AUTOMATION_COMMAND(FSpawnTestAgent());
    ADD_LATENT_AUTOMATION_COMMAND(FExecuteSimpleMoveToActionTaskLatentCommand());
    ADD_LATENT_AUTOMATION_COMMAND(FWaitToArriveAtDestinationLatentCommand(FVector(200.f, 0.0f, 40.0f)));
    ADD_LATENT_AUTOMATION_COMMAND(FEndPlayMapCommand());
    return true;
}

#endif
