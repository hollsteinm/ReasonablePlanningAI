// Radaway Software LLC. 2025. All rights reserved.


#include "Composer/ActionTasks/RpaiActionTask_EnvQuery.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Direction.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"
#include "Core/RpaiState.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "AISystem.h"

FActionTaskEnvQueryMemory::FActionTaskEnvQueryMemory()
	: RequestId(INDEX_NONE)
{

}

URpaiActionTask_EnvQuery::URpaiActionTask_EnvQuery()
{
	bCompleteAfterStart = false;
	ActionTaskMemoryStructType = FActionTaskEnvQueryMemory::StaticStruct();
}

void URpaiActionTask_EnvQuery::ReceiveStartActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	check(ActionInstigator != nullptr);
	check(CurrentState != nullptr);

	FActionTaskEnvQueryMemory* Memory = ActionMemory.Get<FActionTaskEnvQueryMemory>();
	Memory->RequestId = INDEX_NONE;

	if (QueryTemplate != nullptr)
	{
		FEnvQueryRequest Request(QueryTemplate, ActionInstigator->GetPawn());
		if (QueryConfig.Num() > 0)
		{
			for (auto& Param : QueryConfig)
			{
				Request.SetDynamicParam(Param);
			}
		}

		Memory->RequestId = Request.Execute(RunMode, FQueryFinishedSignature::CreateUObject(this, &URpaiActionTask_EnvQuery::OnQueryFinished, ActionInstigator, CurrentState, ActionMemory));
		if (Memory->RequestId >= 0)
		{
			if (UBrainComponent* AIBrain = ActionInstigator->GetBrainComponent())
			{
				if (ObserverCache.Num() <= Memory->RequestId)
				{
					ObserverCache.SetNum(Memory->RequestId + 8 /*Arbitrarily grow by this amount*/, false);
				}
				ObserverCache[Memory->RequestId] = FAIMessageObserver::Create(AIBrain, UBrainComponent::AIMessage_QueryFinished, Memory->RequestId, FOnAIMessage::CreateUObject(this, &URpaiActionTask_EnvQuery::OnAIMessage, ActionInstigator, CurrentState, ActionMemory));
			}
		}
	}
}

void URpaiActionTask_EnvQuery::ReceiveCancelActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld, bool bCancelShouldExitPlan)
{
	FActionTaskEnvQueryMemory* Memory = ActionMemory.Get<FActionTaskEnvQueryMemory>();
	if (Memory->RequestId != INDEX_NONE)
	{
		auto World = ActionInstigator->GetWorld();
		if (auto QueryManager = UEnvQueryManager::GetCurrent(World))
		{
			QueryManager->AbortQuery(Memory->RequestId);
		}
	}
}

void URpaiActionTask_EnvQuery::OnQueryFinished(TSharedPtr<FEnvQueryResult> Result, AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory)
{
	if (Result->IsAborted())
	{
		ObserverCache.RemoveAt(Result->QueryID);
		return;
	}

	bool bSuccess = Result->IsSuccessful() && Result->Items.Num() > 0;

	if (bSuccess)
	{
		uint8* Data = Result->RawData.GetData() + Result->Items[0].DataOffset;
		auto ItemTypeCDO = Result->ItemType->GetDefaultObject<UEnvQueryItemType>();

		//Need to use reflection here (which sucks) because the interface is tightly
		//coupled to blackboard. That is unless we use a blackboard state object.
		if (UEnvQueryItemType_Actor::StaticClass() == *Result->ItemType)
		{
			auto ActorItem = Cast<UEnvQueryItemType_Actor>(ItemTypeCDO);
			if (ActionTaskStateKeyValueReference.ExpectedValueType == EStatePropertyType::Object)
			{
				CurrentState->SetObject(ActionTaskStateKeyValueReference.StateKeyName, ActorItem->GetActor(Data));
			}
			else if (ActionTaskStateKeyValueReference.ExpectedValueType == EStatePropertyType::Vector)
			{
				CurrentState->SetValueOfType(ActionTaskStateKeyValueReference.StateKeyName, ActorItem->GetActor(Data)->GetActorLocation());
			}
			else
			{
				bSuccess = false;
			}
		}
		else if (UEnvQueryItemType_Point::StaticClass() == *Result->ItemType && ActionTaskStateKeyValueReference.ExpectedValueType == EStatePropertyType::Vector)
		{
			auto PointItem = Cast<UEnvQueryItemType_Point>(ItemTypeCDO);
			CurrentState->SetVector(ActionTaskStateKeyValueReference.StateKeyName, PointItem->GetItemLocation(Data));

		}
		else if (UEnvQueryItemType_Direction::StaticClass() == *Result->ItemType && ActionTaskStateKeyValueReference.ExpectedValueType == EStatePropertyType::Rotator)
		{
			auto DirectionItem = Cast<UEnvQueryItemType_Direction>(ItemTypeCDO);
			CurrentState->SetRotator(ActionTaskStateKeyValueReference.StateKeyName, DirectionItem->GetItemRotation(Data));
		}
		else
		{
			bSuccess = false;
		}
	}

	if (UBrainComponent* AIBrain = ActionInstigator->GetBrainComponent())
	{
		FAIMessage::Send(AIBrain, FAIMessage(UBrainComponent::AIMessage_QueryFinished, this, Result->QueryID, bSuccess));
	}
}

void URpaiActionTask_EnvQuery::OnAIMessage(UBrainComponent* BrainComp, const FAIMessage& Message, AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory)
{
	if (Message.Status == FAIMessage::Success)
	{
		CompleteActionTask(ActionInstigator, CurrentState, ActionMemory);
	}
	else
	{
		CancelActionTask(ActionInstigator, CurrentState, ActionMemory);
	}
	ObserverCache.RemoveAt(Message.RequestID);
}
