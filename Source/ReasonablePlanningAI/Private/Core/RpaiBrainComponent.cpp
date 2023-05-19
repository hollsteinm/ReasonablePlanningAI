// Troll Purse. All rights reserved.


#include "Core/RpaiBrainComponent.h"
#include "Core/RpaiTypes.h"
#include "Core/RpaiActionBase.h"
#include "Core/RpaiGoalBase.h"
#include "Core/RpaiReasonerBase.h"
#include "Core/RpaiPlannerBase.h"
#include "Core/RpaiState.h"
#include "VisualLogger/VisualLoggerTypes.h"
#include "VisualLogger/VisualLogger.h"
#include "AIController.h"

static const FRpaiMemory::MemorySizeType DefaultBlockSize = 256;

URpaiBrainComponent::URpaiBrainComponent()
	: CurrentAction(nullptr)
	, PlannedActions({})
	, CurrentGoal(nullptr)
	, CachedStateInstance(nullptr)
	, bIsPaused(false)
	, ComponentActionMemory(DefaultBlockSize)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URpaiBrainComponent::BeginPlay()
{
	Super::BeginPlay();
	if (AIOwner == nullptr)
	{
		if (APawn* OwnerPawn = CastChecked<APawn>(GetOwner()))
		{
			AIOwner = CastChecked<AAIController>(OwnerPawn->GetController());
		}
	}
}

void URpaiBrainComponent::EndPlay(EEndPlayReason::Type Reason)
{
	Super::EndPlay(Reason);
	StopLogic("End Play");
	auto& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.ClearAllTimersForObject(this);
}

void URpaiBrainComponent::UnregisterOldAction(URpaiActionBase* OldAction)
{
	if (OldAction == nullptr)
	{
		return;
	}

	OldAction->OnActionComplete.RemoveAll(this);
	OldAction->OnActionCancelled.RemoveAll(this);
}

void URpaiBrainComponent::RegisterNewAction(URpaiActionBase* NewAction)
{
	if (NewAction == nullptr)
	{
		return;
	}

	CurrentActionMemory = NewAction->AllocateMemorySlice(ComponentActionMemory);
	NewAction->OnActionComplete.AddUniqueDynamic(this, &URpaiBrainComponent::OnActionCompleted);
	NewAction->OnActionCancelled.AddUniqueDynamic(this, &URpaiBrainComponent::OnActionCancelled);
}

void URpaiBrainComponent::PopNextAction()
{
	if (PlannedActions.Num() > 0)
	{
		UnregisterOldAction(CurrentAction);
		CurrentAction = PlannedActions.Pop();
		check(CurrentAction != nullptr);
		RegisterNewAction(CurrentAction);
		CurrentAction->StartAction(AIOwner, LoadOrCreateStateFromAi(), CurrentActionMemory, AIOwner->GetPawn(), AIOwner->GetWorld());
		UE_VLOG(GetOwner(), LogRpai, Log, TEXT("Action Started %s"), *CurrentAction->GetActionName());
	}
}

void URpaiBrainComponent::OnActionCompleted(URpaiActionBase* CompletedAction, AAIController* ActionInstigator, URpaiState* CompletedOnState)
{
	if (CompletedOnState != LoadOrCreateStateFromAi())
	{
		//log
		return;
	}

	if (CompletedAction != CurrentAction)
	{
		//log
		return;
	}

	if (ActionInstigator != AIOwner)
	{
		//log
		return;
	}

	UE_VLOG(GetOwner(), LogRpai, Log, TEXT("Action Completed %s"), *CompletedAction->GetActionName());
	PopNextAction();
}

void URpaiBrainComponent::OnActionCancelled(URpaiActionBase* CancelledAction, AAIController* ActionInstigator, URpaiState* CompletedOnState, bool bCancelShouldExitPlan)
{
	if (CompletedOnState != LoadOrCreateStateFromAi())
	{
		//log
		return;
	}

	if (CancelledAction != CurrentAction)
	{
		//log
		return;
	}

	if (ActionInstigator != AIOwner)
	{
		//log
		return;
	}

	UE_VLOG(GetOwner(), LogRpai, Log, TEXT("Action Cancelled %s"), *CancelledAction->GetActionName());
	if (bCancelShouldExitPlan)
	{
		PlannedActions.Empty();
	}
	else
	{
		PopNextAction();
	}
}

void URpaiBrainComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bIsPaused)
	{
		return;
	}

	if (CurrentAction != nullptr)
	{
		CurrentAction->UpdateAction(AIOwner, LoadOrCreateStateFromAi(), DeltaTime, CurrentActionMemory, AIOwner->GetPawn(), AIOwner->GetWorld());
	}
	else if (PlannedActions.Num() <= 0)
	{
		StartLogic();
	}
}

const URpaiReasonerBase* URpaiBrainComponent::AcquireReasoner_Implementation()
{
	return nullptr;
}

const URpaiPlannerBase* URpaiBrainComponent::AcquirePlanner_Implementation()
{
	return nullptr;
}

void URpaiBrainComponent::AcquireGoals_Implementation(TArray<URpaiGoalBase*>& OutGoals)
{
	OutGoals.Empty();
}

void URpaiBrainComponent::AcquireActions_Implementation(TArray<URpaiActionBase*>& OutActions)
{
	OutActions.Empty();
}

TSubclassOf<URpaiState> URpaiBrainComponent::GetStateType_Implementation()
{
	return URpaiState::StaticClass();
}

void URpaiBrainComponent::StartLogic()
{
	UE_VLOG(GetOwner(), LogRpai, Log, TEXT("%s"), ANSI_TO_TCHAR(__FUNCTION__));
	if (CurrentAction != nullptr && CurrentGoal != nullptr)
	{
		UE_VLOG(GetOwner(), LogRpai, Log, TEXT("%s: Skipping, logic already started."), ANSI_TO_TCHAR(__FUNCTION__));
		return;
	}

	auto Reasoner = AcquireReasoner();
	auto Planner = AcquirePlanner();
	if (Reasoner == nullptr || Planner == nullptr)
	{
		return;
	}

	TArray<URpaiGoalBase*> Goals;
	AcquireGoals(Goals);

	TArray<URpaiActionBase*> Actions;
	AcquireActions(Actions);

	check(Reasoner != nullptr);
	check(Planner != nullptr);
	CurrentGoal = Reasoner->ReasonNextGoal(Goals, LoadOrCreateStateFromAi());

	if (CurrentGoal != nullptr && Planner->PlanChosenGoal(CurrentGoal, LoadOrCreateStateFromAi(), Actions, PlannedActions))
	{
		UE_VLOG(GetOwner(), LogRpai, Log, TEXT("Goal Planned %s"), *CurrentGoal->GetGoalName());
		PopNextAction();
	}
	else
	{
		auto& TimerManager = GetWorld()->GetTimerManager();

		if (TimerManager.GetTimerRemaining(PlanningDebounce) > 0.f)
		{
			return;
		}

		TimerManager.ClearTimer(PlanningDebounce);
		TimerManager.SetTimer(PlanningDebounce, this, &URpaiBrainComponent::StartLogic, 1.f);
	}
}

void URpaiBrainComponent::RestartLogic()
{
	UE_VLOG(GetOwner(), LogRpai, Log, TEXT("%s"), ANSI_TO_TCHAR(__FUNCTION__));
	StopLogic("Restart Logic");
	Cleanup();
	StartLogic();
}

void URpaiBrainComponent::StopLogic(const FString& Reason)
{
	UE_VLOG(GetOwner(), LogRpai, Log, TEXT("Stopping Reasonable Planning, reason: \'%s\'"), *Reason);
	PlannedActions.Empty();
	if (CurrentAction != nullptr)
	{
		UnregisterOldAction(CurrentAction);
		CurrentAction->CancelAction(AIOwner, LoadOrCreateStateFromAi(), CurrentActionMemory, AIOwner->GetPawn(), AIOwner->GetWorld());
	}
	CurrentAction = nullptr;
}

void URpaiBrainComponent::Cleanup()
{
	PlannedActions.Empty();
	CurrentAction = nullptr;
}

void URpaiBrainComponent::PauseLogic(const FString& Reason)
{
	UE_VLOG(GetOwner(), LogRpai, Log, TEXT("Execution updates: PAUSED (%s)"), *Reason);
	bIsPaused = true;
}

EAILogicResuming::Type URpaiBrainComponent::ResumeLogic(const FString& Reason)
{
	UE_VLOG(GetOwner(), LogRpai, Log, TEXT("Execution updates: RESUMED (%s)"), *Reason);
	bIsPaused = false;
	if (CurrentAction == nullptr || CurrentGoal == nullptr)
	{
		RestartLogic();
		return EAILogicResuming::RestartedInstead;
	}
	return EAILogicResuming::Continue;
}

URpaiState* URpaiBrainComponent::LoadOrCreateStateFromAi()
{
	if (CachedStateInstance == nullptr)
	{
		CachedStateInstance = NewObject<URpaiState>(GetAIOwner(), *GetStateType());
	}
	SetStateFromAi(CachedStateInstance);
	return CachedStateInstance;
}

void URpaiBrainComponent::SetStateFromAi_Implementation(URpaiState* StateToModify) const
{
	if (auto AI = GetAIOwner())
	{
		StateToModify->SetStateFromController(AI);
	}
}

FString URpaiBrainComponent::GetDebugInfoString() const
{
	FString DebugInfo = Super::GetDebugInfoString();

	auto CurrentGoalString = CurrentGoal == nullptr ? "none" : CurrentGoal->GetName().Append(":").Append(CurrentGoal->GetGoalName());
	auto CurrentActionString = CurrentAction == nullptr ? "none" : CurrentAction->GetName().Append(":").Append(CurrentAction->GetActionName());

	DebugInfo += FString::Printf(TEXT("Goal: %s\n"), *CurrentGoalString);
	DebugInfo += FString::Printf(TEXT("Action: %s\n"), *CurrentActionString);
	DebugInfo += FString::Printf(TEXT("Remaining Planned Actions (%i):\n"), PlannedActions.Num());
	for (const auto PlannedAction : PlannedActions)
	{
		DebugInfo += FString::Printf(TEXT("\t%s\n"), *FString(PlannedAction->GetName().Append(":").Append(PlannedAction->GetActionName())));
	}
	return DebugInfo;
}