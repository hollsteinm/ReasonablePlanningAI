// Troll Purse. All rights reserved.


#include "Composer/ActionTasks/RpaiActionTask_MoveTo.h"
#include "Core/RpaiState.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "AISystem.h"

URpaiActionTask_MoveTo::URpaiActionTask_MoveTo()
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

void URpaiActionTask_MoveTo::ReceiveStartActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	if (!ActionInstigator->ShouldPostponePathUpdates())
	{
		StartMoveActionTask(ActionInstigator, CurrentState, ActionMemory);
	}
}

void URpaiActionTask_MoveTo::ReceiveUpdateActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, float DeltaSeconds, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	UPathFollowingComponent* PathFollowingComponent = ActionInstigator->GetPathFollowingComponent();
	if (!ActionInstigator->ShouldPostponePathUpdates() && PathFollowingComponent && PathFollowingComponent->GetStatus() == EPathFollowingStatus::Idle)
	{
		StartMoveActionTask(ActionInstigator, CurrentState, ActionMemory);
	}
}

void URpaiActionTask_MoveTo::ReceiveCancelActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld, bool bCancelShouldExitPlan)
{
	UPathFollowingComponent* PathFollowingComponent = ActionInstigator->GetPathFollowingComponent();
	if (PathFollowingComponent && PathFollowingComponent->GetStatus() != EPathFollowingStatus::Idle)
	{
		PathFollowingComponent->AbortMove(*this, FPathFollowingResultFlags::ForcedScript | FPathFollowingResultFlags::NewRequest, FAIRequestID::CurrentRequest, EPathFollowingVelocityMode::Keep);
	}
}

void URpaiActionTask_MoveTo::StartMoveActionTask(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory)
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
			CancelActionTask(ActionInstigator, CurrentState, ActionMemory);
			return;
		}

		AActor* TargetActor = Cast<AActor>(MaybeActor);
		if (TargetActor == nullptr)
		{
			CancelActionTask(ActionInstigator, CurrentState, ActionMemory);
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
				auto MoveHandle = FAIMessageObserver::Create(AIBrain, UBrainComponent::AIMessage_MoveFinished, MoveRequest.MoveId, FOnAIMessage::CreateUObject(this, &URpaiActionTask_MoveTo::OnAIMessage, ActionInstigator, CurrentState, ActionMemory));
				auto RepathHandle = FAIMessageObserver::Create(AIBrain, UBrainComponent::AIMessage_RepathFailed, MoveRequest.MoveId, FOnAIMessage::CreateUObject(this, &URpaiActionTask_MoveTo::OnAIMessage, ActionInstigator, CurrentState, ActionMemory));
				MoveFinishedHandles.Add(MoveRequest.MoveId, MoveHandle);
				RepathFailedHandles.Add(MoveRequest.MoveId, RepathHandle);
			}
		}
		break;
		case EPathFollowingRequestResult::Failed:
		case EPathFollowingRequestResult::AlreadyAtGoal:
		default:
			CompleteActionTask(ActionInstigator, CurrentState, ActionMemory);
			break;
		}
	}
}

void URpaiActionTask_MoveTo::OnAIMessage(UBrainComponent* BrainComp, const FAIMessage& Message, AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory)
{
	MoveFinishedHandles.Remove(Message.RequestID);
	RepathFailedHandles.Remove(Message.RequestID);
	if (Message.Status == FAIMessage::Success)
	{
		CompleteActionTask(ActionInstigator, CurrentState, ActionMemory);
	}
	else
	{
		CancelActionTask(ActionInstigator, CurrentState, ActionMemory);
	}
}
