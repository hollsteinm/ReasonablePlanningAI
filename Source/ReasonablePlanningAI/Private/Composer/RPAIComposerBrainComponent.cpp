// Radaway Software LLC. 2025. All rights reserved.


#include "Composer/RpaiComposerBrainComponent.h"
#include "Engine/World.h"
#include "Composer/RpaiComposerBehavior.h"
#include "AIController.h"

URpaiComposerBrainComponent::URpaiComposerBrainComponent()
   : ReasonablePlanningBehavior(nullptr)
{
}

void URpaiComposerBrainComponent::SetReasonablePlanningBehavior(URpaiComposerBehavior* NewBehavior)
{
   if (NewBehavior != ReasonablePlanningBehavior)
   {
      auto World = GetWorld();
      if (World == nullptr || !World->IsGameWorld())
      {
         ReasonablePlanningBehavior = NewBehavior;
         DefaultStateType = NewBehavior->GetConstructedStateType();
         ClearCachedStateInstance();
      }
      else
      {
         if (ReasonablePlanningBehavior != nullptr)
         {
            StopLogic("New Behavior");
         }
         ReasonablePlanningBehavior = NewBehavior;
         DefaultStateType = NewBehavior->GetConstructedStateType();
         ClearCachedStateInstance();
         StartLogic();
      }
   }
}

void URpaiComposerBrainComponent::OnComponentSetStateFromAi(URpaiState* StateToModify) const
{
   if (IsValid(ReasonablePlanningBehavior))
   {
      const AAIController* AI = GetAIOwner();
      if (FAISystem::IsValidControllerAndHasValidPawn(AI))
      {
         ReasonablePlanningBehavior->Bindings.Transfer(AI, StateToModify);
         ReasonablePlanningBehavior->Bindings.Transfer(AI->GetPawn(), StateToModify);
      }
   }
}

void URpaiComposerBrainComponent::OnRegister()
{
   Super::OnRegister();
   if (IsValid(ReasonablePlanningBehavior))
   {
      DefaultStateType = ReasonablePlanningBehavior->GetConstructedStateType();
   }
}

#if WITH_EDITOR
void URpaiComposerBrainComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
   Super::PostEditChangeProperty(PropertyChangedEvent);
   static const FName NAME_ReasonablePlanningBehavior = GET_MEMBER_NAME_CHECKED(URpaiComposerBrainComponent, ReasonablePlanningBehavior);

   const FName ChangedPropertyName = PropertyChangedEvent.GetPropertyName();

   if (ChangedPropertyName == NAME_ReasonablePlanningBehavior)
   {
      DefaultStateType = ReasonablePlanningBehavior->GetConstructedStateType();
   }
}
#endif

const URpaiReasonerBase* URpaiComposerBrainComponent::AcquireReasoner_Implementation()
{
   return ReasonablePlanningBehavior == nullptr ? nullptr : ReasonablePlanningBehavior->GetReasoner();
}

const URpaiPlannerBase* URpaiComposerBrainComponent::AcquirePlanner_Implementation()
{
   return ReasonablePlanningBehavior == nullptr ? nullptr : ReasonablePlanningBehavior->GetPlanner();
}

void URpaiComposerBrainComponent::AcquireGoals_Implementation(TArray<URpaiGoalBase*>& OutGoals)
{
   OutGoals.Empty();
   if (ReasonablePlanningBehavior != nullptr)
   {
      OutGoals = ReasonablePlanningBehavior->GetGoals();
   }
}

void URpaiComposerBrainComponent::AcquireActions_Implementation(TArray<URpaiActionBase*>& OutActions)
{
   OutActions.Empty();
   if (ReasonablePlanningBehavior != nullptr)
   {
      OutActions = ReasonablePlanningBehavior->GetActions();
   }
}
