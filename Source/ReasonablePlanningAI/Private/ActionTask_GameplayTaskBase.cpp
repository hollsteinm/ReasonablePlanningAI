// Troll Purse. All rights reserved.


#include "ActionTask_GameplayTaskBase.h"
#include "ReasonablePlanningTypes.h"
#include "GameplayTasksComponent.h"
#include "Tasks/AITask.h"
#include "AIController.h"
#include "AITypes.h"

UActionTask_GameplayTaskBase::UActionTask_GameplayTaskBase()
{
	bCompleteAfterStart = false;
}

void UActionTask_GameplayTaskBase::ReceiveCancelActionTask_Implementation(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	if (!bCompleteAfterStart)
	{
		UAITask* TheTask = nullptr;
		if (TaskToState.RemoveAndCopyValue(CurrentState, TheTask))
		{
			if (!TheTask->IsFinished())
			{
				TheTask->ExternalCancel();
			}
		}
	}
	else
	{
		TaskToState.Remove(CurrentState);
	}
}

void UActionTask_GameplayTaskBase::ReceiveCompleteActionTask_Implementation(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	TaskToState.Remove(CurrentState);
}


UGameplayTasksComponent* UActionTask_GameplayTaskBase::GetGameplayTasksComponent(const UGameplayTask& Task) const
{
	const UAITask* AITask = Cast<const UAITask>(&Task);
	return (AITask && AITask->GetAIController()) ? AITask->GetAIController()->GetGameplayTasksComponent(Task) : Task.GetGameplayTasksComponent();
}

AActor* UActionTask_GameplayTaskBase::GetGameplayTaskOwner(const UGameplayTask* Task) const
{
	if (Task == nullptr)
	{
		return nullptr;
	}

	const UAITask* AITask = Cast<const UAITask>(Task);
	if (AITask)
	{
		return AITask->GetAIController();
	}

	const UGameplayTasksComponent* TasksComponent = Task->GetGameplayTasksComponent();
	return TasksComponent ? TasksComponent->GetGameplayTaskOwner(Task) : nullptr;
}

AActor* UActionTask_GameplayTaskBase::GetGameplayTaskAvatar(const UGameplayTask* Task) const
{
	if (Task == nullptr)
	{
		return nullptr;
	}

	const UAITask* AITask = Cast<const UAITask>(Task);
	if (AITask)
	{
		return AITask->GetAIController() ? AITask->GetAIController()->GetPawn() : nullptr;
	}

	const UGameplayTasksComponent* TasksComponent = Task->GetGameplayTasksComponent();
	return TasksComponent ? TasksComponent->GetGameplayTaskAvatar(Task) : nullptr;
}

uint8 UActionTask_GameplayTaskBase::GetGameplayTaskDefaultPriority() const
{
	return static_cast<uint8>(EAITaskPriority::AutonomousAI);
}

void UActionTask_GameplayTaskBase::OnGameplayTaskInitialized(UGameplayTask& Task)
{
	const UAITask* AITask = Cast<const UAITask>(&Task);
	if (AITask && (AITask->GetAIController() == nullptr))
	{
		UE_LOG(LogRPAI, Error, TEXT("Missing AIController in AITask %s"), *AITask->GetName());
	}
}

void UActionTask_GameplayTaskBase::OnGameplayTaskDeactivated(UGameplayTask& Task)
{
	UAITask* AITask = Cast<UAITask>(&Task);
	if (AITask && AITask->GetAIController() && AITask->GetState() != EGameplayTaskState::Paused)
	{
		CompleteActionTask(AITask->GetAIController(), nullptr);
	}
}

UAITask* UActionTask_GameplayTaskBase::GetTaskForState(const UReasonablePlanningState* CurrentState)
{
	auto Result = TaskToState.Find(CurrentState);
	return Result == nullptr ? nullptr : *Result;
}

void UActionTask_GameplayTaskBase::StartTask(UReasonablePlanningState* CurrentState, UAITask* TaskToStart)
{
	auto CurrentStateTask = TaskToState.Find(CurrentState);
	if (CurrentStateTask != nullptr)
	{
		CancelActionTask(Cast<AAIController>(GetGameplayTaskOwner(*CurrentStateTask)), CurrentState);
	}

	TaskToStart->ReadyForActivation();
	TaskToState.Emplace(CurrentState, TaskToStart);
	if (TaskToStart->GetState() == EGameplayTaskState::Finished)
	{
		CompleteActionTask(Cast<AAIController>(GetGameplayTaskOwner(*CurrentStateTask)), CurrentState);
	}
}