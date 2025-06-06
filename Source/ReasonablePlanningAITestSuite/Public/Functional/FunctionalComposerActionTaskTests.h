// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.

#pragma once

#include "AIController.h"
#include "GameFramework/Character.h"
#include "Core/RpaiBrainComponent.h"
#include "Composer/RpaiComposerBrainComponent.h"
#include "Composer/RpaiComposerActionTaskBase.h"
#include "FunctionalComposerActionTaskTests.generated.h"

UCLASS(HideDropdown)
class AFunctionalTestAutomationAgentCharacter : public ACharacter
{
   GENERATED_BODY()

public:
   AFunctionalTestAutomationAgentCharacter();
};

UCLASS(HideDropdown)
class AFunctionalTestAutomationAgentController : public AAIController
{
   GENERATED_BODY()
protected:
   URpaiComposerBrainComponent* RpaiBrainComponent;

public:
   AFunctionalTestAutomationAgentController();

   FORCEINLINE URpaiComposerBrainComponent* GetRpaiBrainComponent() const { return CastChecked<URpaiComposerBrainComponent>(GetBrainComponent()); }
};

struct FRpaiAutomationHelpers
{
   /**
   * Safely secures a pointer to the test automation world.
   * Returns the world on success, nullptr if the world was unable to be acquired or is not a game or PIE world.
   **/
   static UWorld* GetAutomationWorld();

   /**
   * Creates an AFunctionalTestAutomationAgentCharacter with a URpaiBrainComponent attached to the owning AFunctionalTestAutomationAgentController.
   * Returns the controller.
   **/
   static AFunctionalTestAutomationAgentController* CreateRpaiAgentWithController(UWorld* AutomationWorld, FVector Location);

   /**
   * Creates and applies and always planned and always selected composer structure for the single Action Task
   **/
   static void CreateSingleActionTaskComposer(URpaiComposerBrainComponent* TargetComposerBrain, URpaiComposerActionTaskBase* AlwaysExecuteAction);
};
