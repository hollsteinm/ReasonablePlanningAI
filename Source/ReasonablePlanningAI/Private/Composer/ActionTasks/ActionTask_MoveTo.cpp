// Troll Purse. All rights reserved.


#include "Composer/ActionTasks/ActionTask_MoveTo.h"
#include "Core/ReasonablePlanningState.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "AISystem.h"

UActionTask_MoveTo::UActionTask_MoveTo()
	: AcceptableRadius(GET_AI_CONFIG_VAR(AcceptanceRadius))
	, bAllowStrafe(GET_AI_CONFIG_VAR(bAllowStrafing))
	, bAllowPartialPath(GET_AI_CONFIG_VAR(bAcceptPartialPaths))
	, bTrackMovingGoal(true)
	, bProjectGoalLocation(true)
	, bReachTestIncludesAgentRadius(GET_AI_CONFIG_VAR(bFinishMoveOnGoalOverlap))
	, bReachTestIncludesGoalRadius(GET_AI_CONFIG_VAR(bFinishMoveOnGoalOverlap))
	, bUsePathfinding(true)
	, bStopOnOverlapNeedsUpdate(true)
{
	ActionTaskStateKeyValueReference.ExpectedValueType = EStatePropertyType::Vector;
	bCompleteAfterStart = false;
}

void UActionTask_MoveTo::ReceiveStartActionTask_Implementation(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	if (!ActionInstigator->ShouldPostponePathUpdates())
	{
		StartMoveActionTask(ActionInstigator, CurrentState);
	}
}

void UActionTask_MoveTo::ReceiveUpdateActionTask_Implementation(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, float DeltaSeconds, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	UPathFollowingComponent* PathFollowingComponent = ActionInstigator->GetPathFollowingComponent();
	if (!ActionInstigator->ShouldPostponePathUpdates() && PathFollowingComponent && PathFollowingComponent->GetStatus() == EPathFollowingStatus::Idle)
	{
		StartMoveActionTask(ActionInstigator, CurrentState);
	}
}

void UActionTask_MoveTo::ReceiveCancelActionTask_Implementation(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	UPathFollowingComponent* PathFollowingComponent = ActionInstigator->GetPathFollowingComponent();
	if (PathFollowingComponent && PathFollowingComponent->GetStatus() != EPathFollowingStatus::Idle)
	{
		PathFollowingComponent->AbortMove(*this, FPathFollowingResultFlags::ForcedScript | FPathFollowingResultFlags::NewRequest, FAIRequestID::CurrentRequest, EPathFollowingVelocityMode::Keep);
	}
}

void UActionTask_MoveTo::StartMoveActionTask(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState)
{
	check(ActionInstigator != nullptr);
	check(CurrentState != nullptr);

	FAIMoveRequest MoveReq;
	MoveReq.SetUsePathfinding(bUsePathfinding);
	MoveReq.SetAllowPartialPath(bAllowPartialPath);
	MoveReq.SetProjectGoalLocation(bProjectGoalLocation);
	MoveReq.SetNavigationFilter(*FilterClass ? FilterClass : ActionInstigator->GetDefaultNavigationFilterClass());
	MoveReq.SetAcceptanceRadius(AcceptableRadius);
	MoveReq.SetReachTestIncludesAgentRadius(bReachTestIncludesGoalRadius && bReachTestIncludesAgentRadius);
	MoveReq.SetCanStrafe(bAllowStrafe);

	FVector TargetLocation;
	if (!CurrentState->GetVector(ActionTaskStateKeyValueReference.StateKeyName, TargetLocation))
	{
		UObject* MaybeActor = nullptr;
		if (!CurrentState->GetObject(ActionTaskStateKeyValueReference.StateKeyName, MaybeActor))
		{
			CancelActionTask(ActionInstigator, CurrentState);
			return;
		}

		AActor* TargetActor = Cast<AActor>(MaybeActor);
		if (TargetActor == nullptr)
		{
			CancelActionTask(ActionInstigator, CurrentState);
			return;
		}
		else if (bTrackMovingGoal)
		{
			MoveReq.SetGoalActor(TargetActor);
		}
		else
		{
			MoveReq.SetGoalLocation(TargetActor->GetActorLocation());
		}
	}
	else
	{
		MoveReq.SetGoalLocation(TargetLocation);
	}

	if (MoveReq.IsValid())
	{
		UPathFollowingComponent* PathFollowingComponent = ActionInstigator->GetPathFollowingComponent();
		if (PathFollowingComponent && PathFollowingComponent->GetStatus() != EPathFollowingStatus::Idle)
		{
			PathFollowingComponent->AbortMove(*this, FPathFollowingResultFlags::ForcedScript | FPathFollowingResultFlags::NewRequest, FAIRequestID::CurrentRequest, EPathFollowingVelocityMode::Keep);
		}

		auto MoveRequest = ActionInstigator->MoveTo(MoveReq);
		switch (MoveRequest.Code)
		{
		case EPathFollowingRequestResult::RequestSuccessful:
		{
			UBrainComponent* AIBrain = ActionInstigator->GetBrainComponent();
			if (AIBrain != nullptr)
			{
				//Returns a handle, but the brain component actually caches it so let us not worry about it.
				FAIMessageObserver::Create(AIBrain, UBrainComponent::AIMessage_MoveFinished, MoveRequest.MoveId, FOnAIMessage::CreateUObject(this, &UActionTask_MoveTo::OnAIMessage, ActionInstigator, CurrentState));
				FAIMessageObserver::Create(AIBrain, UBrainComponent::AIMessage_RepathFailed, FOnAIMessage::CreateUObject(this, &UActionTask_MoveTo::OnAIMessage, ActionInstigator, CurrentState));
			}
		}
		break;
		case EPathFollowingRequestResult::Failed:
		case EPathFollowingRequestResult::AlreadyAtGoal:
		default:
			CompleteActionTask(ActionInstigator, CurrentState);
			break;
		}
	}
}

void UActionTask_MoveTo::OnAIMessage(UBrainComponent* BrainComp, const FAIMessage& Message, AAIController* ActionInstigator, UReasonablePlanningState* CurrentState)
{
	if (Message.Status == FAIMessage::Success)
	{
		CompleteActionTask(ActionInstigator, CurrentState);
	}
	else
	{
		CancelActionTask(ActionInstigator, CurrentState);
	}
}
