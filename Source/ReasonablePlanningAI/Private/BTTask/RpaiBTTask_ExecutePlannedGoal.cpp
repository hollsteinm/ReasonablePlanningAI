#include "BTTask/RpaiBTTask_ExecutePlannedGoal.h"
#include "Composer/RpaiComposerBehavior.h"
#include "Core/RpaiGoalBase.h"
#include "Core/RpaiActionBase.h"
#include "Core/RpaiPlannerBase.h"
#include "States/RpaiState_BlackboardComponentAdapter.h"
#include "VisualLogger/VisualLoggerTypes.h"
#include "VisualLogger/VisualLogger.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BTTask/RpaiStateBlackboardComponent.h"

URpaiBTTask_ExecutePlannedGoal::URpaiBTTask_ExecutePlannedGoal()
	: Planner(nullptr)
	, Actions()
{
	NodeName = "Execute Planned Goal";
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(URpaiBTTask_ExecutePlannedGoal, BlackboardKey), URpaiGoalBase::StaticClass());
	bNotifyTick = true;
}

EBTNodeResult::Type URpaiBTTask_ExecutePlannedGoal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	check(Planner != nullptr);
	check(!Actions.IsEmpty());
	check(NodeMemory != nullptr);

	FExecutePlannedGoalMemory* PlannedGoalMemory = (FExecutePlannedGoalMemory*)NodeMemory;
	PlannedGoalMemory->CurrentAction = nullptr;
	PlannedGoalMemory->RemainingPlan.Empty();

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	URpaiGoalBase* TargetGoal = Cast<URpaiGoalBase>(BlackboardComp->GetValueAsObject(BlackboardKey.SelectedKeyName));
	if (TargetGoal == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	URpaiStateBlackboardComponent* RpaiComp = OwnerComp.GetAIOwner()->FindComponentByClass<URpaiStateBlackboardComponent>();
	if (RpaiComp == nullptr)
	{
		UE_LOG(LogRpai, Error, TEXT("Missing URpaiStateBlackboardComponent"));
		return EBTNodeResult::Failed;
	}

	URpaiState_BlackboardComponentAdapter* BlackboardAdapterState = RpaiComp->GetStateAdapter();
	if (BlackboardAdapterState == nullptr)
	{
		UE_LOG(LogRpai, Error, TEXT("Missing URpaiStateBlackboardComponent::StateAdpater"));
		return EBTNodeResult::Failed;
	}

	bool bCanPlan = Planner->PlanChosenGoal(TargetGoal, BlackboardAdapterState, Actions, PlannedGoalMemory->RemainingPlan);
	if (bCanPlan && PlannedGoalMemory->RemainingPlan.Num() > 0)
	{
		PlannedGoalMemory->CurrentAction = PlannedGoalMemory->RemainingPlan.Pop();
		ControllerToMemory.Add(OwnerComp.GetAIOwner(), PlannedGoalMemory);
		PlannedGoalMemory->CurrentAction->OnActionComplete.AddUniqueDynamic(this, &URpaiBTTask_ExecutePlannedGoal::OnActionCompleted);
		PlannedGoalMemory->CurrentAction->OnActionCancelled.AddUniqueDynamic(this, &URpaiBTTask_ExecutePlannedGoal::OnActionCancelled);
		PlannedGoalMemory->ActionMemory = PlannedGoalMemory->CurrentAction->AllocateMemorySlice(PlannedGoalMemory->ExecutionMemory);
		PlannedGoalMemory->CurrentAction->StartAction(OwnerComp.GetAIOwner(), BlackboardAdapterState, FRpaiMemoryStruct(), OwnerComp.GetOwner(), OwnerComp.GetWorld());
		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Failed;
}

EBTNodeResult::Type URpaiBTTask_ExecutePlannedGoal::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	check(NodeMemory != nullptr);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	check(BlackboardComp != nullptr);

	URpaiStateBlackboardComponent* RpaiComp = OwnerComp.GetAIOwner()->FindComponentByClass<URpaiStateBlackboardComponent>();
	if (RpaiComp == nullptr)
	{
		UE_LOG(LogRpai, Error, TEXT("Missing URpaiStateBlackboardComponent"));
		return EBTNodeResult::Failed;
	}

	URpaiState_BlackboardComponentAdapter* BlackboardAdapterState = RpaiComp->GetStateAdapter();
	if (BlackboardAdapterState == nullptr)
	{
		UE_LOG(LogRpai, Error, TEXT("Missing URpaiStateBlackboardComponent::StateAdpater"));
		return EBTNodeResult::Failed;
	}

	FExecutePlannedGoalMemory* PlannedGoalMemory = (FExecutePlannedGoalMemory*)NodeMemory;
	if (PlannedGoalMemory->CurrentAction != nullptr)
	{
		PlannedGoalMemory->CurrentAction->CancelAction(OwnerComp.GetAIOwner(), BlackboardAdapterState, FRpaiMemoryStruct(), OwnerComp.GetOwner(), OwnerComp.GetWorld());
	}

	PlannedGoalMemory->CurrentAction = nullptr;
	PlannedGoalMemory->RemainingPlan.Empty();
	return EBTNodeResult::Aborted;
}

void URpaiBTTask_ExecutePlannedGoal::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	check(NodeMemory != nullptr);
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	check(BlackboardComp != nullptr);

	URpaiStateBlackboardComponent* RpaiComp = OwnerComp.GetAIOwner()->FindComponentByClass<URpaiStateBlackboardComponent>();
	if (RpaiComp == nullptr)
	{
		UE_LOG(LogRpai, Error, TEXT("Missing URpaiStateBlackboardComponent"));
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	URpaiState_BlackboardComponentAdapter* BlackboardAdapterState = RpaiComp->GetStateAdapter();
	if (BlackboardAdapterState == nullptr)
	{
		UE_LOG(LogRpai, Error, TEXT("Missing URpaiStateBlackboardComponent::StateAdpater"));
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	FExecutePlannedGoalMemory* PlannedGoalMemory = (FExecutePlannedGoalMemory*)NodeMemory;
	if (PlannedGoalMemory->CurrentAction != nullptr)
	{
		PlannedGoalMemory->CurrentAction->UpdateAction(OwnerComp.GetAIOwner(), BlackboardAdapterState, DeltaSeconds, FRpaiMemoryStruct(), OwnerComp.GetOwner(), OwnerComp.GetWorld());
	}

	if (PlannedGoalMemory->CurrentAction == nullptr && PlannedGoalMemory->RemainingPlan.IsEmpty())
	{
		ControllerToMemory.Remove(OwnerComp.GetAIOwner());
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

uint16 URpaiBTTask_ExecutePlannedGoal::GetInstanceMemorySize() const
{
	return sizeof(FExecutePlannedGoalMemory);
}

void URpaiBTTask_ExecutePlannedGoal::DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const
{
	Super::DescribeRuntimeValues(OwnerComp, NodeMemory, Verbosity, Values);

	const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp != nullptr)
	{
		const FString KeyValue = BlackboardComp->DescribeKeyValue(BlackboardKey.GetSelectedKeyID(), EBlackboardDescription::DetailedKeyWithValue);
		Values.Add(FString::Printf(TEXT("Current State: %s"), *KeyValue));
	}

	if (NodeMemory != nullptr)
	{
		FExecutePlannedGoalMemory* PlannedGoalMemory = (FExecutePlannedGoalMemory*)NodeMemory;
		if (PlannedGoalMemory->CurrentAction != nullptr)
		{
			const FString ActionName = PlannedGoalMemory->CurrentAction->GetActionName();
			Values.Add(FString::Printf(TEXT("Current Action: %s"), *ActionName));
		}
		Values.Add(FString::Printf(TEXT("Planned Actions Remaining: %d"), PlannedGoalMemory->RemainingPlan.Num()));
	}
}

FString URpaiBTTask_ExecutePlannedGoal::GetStaticDescription() const
{
	FString KeyDesc("invalid");
	FString InputKeyDesc("invalid");
	FString PlannerDesc("invalid");

	if (BlackboardKey.IsSet())
	{
		KeyDesc = BlackboardKey.SelectedKeyName.ToString();
	}

	if (Planner != nullptr)
	{
		PlannerDesc = Planner->GetName();
	}

	return FString::Printf(TEXT("%s: use %s state to determine plan for goal %s using planner %s"), *Super::GetStaticDescription(), *KeyDesc, *InputKeyDesc, *PlannerDesc);
}

FName URpaiBTTask_ExecutePlannedGoal::GetNodeIconName() const
{
	return FName("BTEditor.Graph.BTNode.Task.RunBehavior.Icon");
}

void URpaiBTTask_ExecutePlannedGoal::OnActionCompleted(URpaiActionBase* CompletedAction, AAIController* ActionInstigator, URpaiState* CompletedOnState)
{
	auto Memory = ControllerToMemory.FindChecked(ActionInstigator);
	OnActionEvent(CompletedAction, ActionInstigator, CompletedOnState, true, Memory);
}

void URpaiBTTask_ExecutePlannedGoal::OnActionCancelled(URpaiActionBase* CompletedAction, AAIController* ActionInstigator, URpaiState* CompletedOnState)
{
	auto Memory = ControllerToMemory.FindChecked(ActionInstigator);
	OnActionEvent(CompletedAction, ActionInstigator, CompletedOnState, false, Memory);
}

void URpaiBTTask_ExecutePlannedGoal::OnActionEvent(URpaiActionBase* CompletedAction, AAIController* ActionInstigator, URpaiState* CompletedOnState, bool bCompleted, FExecutePlannedGoalMemory* TaskMemory)
{
	check(TaskMemory != nullptr)
	check(CompletedAction == TaskMemory->CurrentAction);

	if (CompletedAction->IsValidLowLevel())
	{
		CompletedAction->OnActionCancelled.RemoveAll(this);
		CompletedAction->OnActionComplete.RemoveAll(this);
	}

	if (bCompleted && !TaskMemory->RemainingPlan.IsEmpty())
	{
		TaskMemory->CurrentAction = TaskMemory->RemainingPlan.Pop();
		TaskMemory->CurrentAction->OnActionComplete.AddUniqueDynamic(this, &URpaiBTTask_ExecutePlannedGoal::OnActionCompleted);
		TaskMemory->CurrentAction->OnActionCancelled.AddUniqueDynamic(this, &URpaiBTTask_ExecutePlannedGoal::OnActionCancelled);
		TaskMemory->ActionMemory = TaskMemory->CurrentAction->AllocateMemorySlice(TaskMemory->ExecutionMemory);
		TaskMemory->CurrentAction->StartAction(ActionInstigator, CompletedOnState, FRpaiMemoryStruct(), ActionInstigator->GetOwner(), ActionInstigator->GetWorld());
	}
	else
	{
		TaskMemory->CurrentAction = nullptr;
	}
}