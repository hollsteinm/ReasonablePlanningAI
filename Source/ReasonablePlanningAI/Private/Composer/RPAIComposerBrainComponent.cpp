// Troll Purse. All rights reserved.


#include "Composer/RpaiComposerBrainComponent.h"
#include "Composer/RpaiComposerBehavior.h"

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
		}
		else
		{
			if (ReasonablePlanningBehavior != nullptr)
			{
				StopLogic("New Behavior");
			}
			ReasonablePlanningBehavior = NewBehavior;
			StartLogic();
		}
	}
}

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

TSubclassOf<URpaiState> URpaiComposerBrainComponent::GetStateType_Implementation()
{
	return ReasonablePlanningBehavior == nullptr ? URpaiState::StaticClass() : ReasonablePlanningBehavior->GetConstructedStateType();
}
