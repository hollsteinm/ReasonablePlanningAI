// Troll Purse. All rights reserved.


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

URpaiActionTask_EnvQuery::URpaiActionTask_EnvQuery()
{
	bCompleteAfterStart = false;
}

void URpaiActionTask_EnvQuery::ReceiveStartActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	check(ActionInstigator != nullptr);
	check(CurrentState != nullptr);

	auto& RequestId = QueryRequestToState.FindOrAdd(CurrentState, INDEX_NONE);
	if (RequestId == INDEX_NONE)
	{
		//Execute
		if (QueryTemplate != nullptr)
		{
			FEnvQueryRequest Request(QueryTemplate, this);
			if (QueryConfig.Num() > 0)
			{
				for (auto& Param : QueryConfig)
				{
					Request.SetDynamicParam(Param);
				}
			}

			RequestId = Request.Execute(RunMode, FQueryFinishedSignature::CreateUObject(this, &URpaiActionTask_EnvQuery::OnQueryFinished, ActionInstigator, CurrentState));
			if (RequestId >= 0)
			{
				if (UBrainComponent* AIBrain = ActionInstigator->GetBrainComponent())
				{
					FAIMessageObserver::Create(AIBrain, UBrainComponent::AIMessage_QueryFinished, RequestId, FOnAIMessage::CreateUObject(this, &URpaiActionTask_EnvQuery::OnAIMessage, ActionInstigator, CurrentState));
				}
			}
		}
	}
}

void URpaiActionTask_EnvQuery::ReceiveCancelActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	int32 RequestId;
	if (QueryRequestToState.RemoveAndCopyValue(CurrentState, RequestId))
	{
		auto World = ActionInstigator->GetWorld();
		if (auto QueryManager = UEnvQueryManager::GetCurrent(World))
		{
			QueryManager->AbortQuery(RequestId);
		}
	}
}

void URpaiActionTask_EnvQuery::ReceiveCompleteActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	QueryRequestToState.Remove(CurrentState);
}

void URpaiActionTask_EnvQuery::OnQueryFinished(TSharedPtr<FEnvQueryResult> Result, AAIController* ActionInstigator, URpaiState* CurrentState)
{
	if (Result->IsAborted())
	{
		return;
	}

	uint8* Data = Result->RawData.GetData() + Result->Items[0].DataOffset;
	auto ItemTypeCDO = Result->ItemType->GetDefaultObject<UEnvQueryItemType>();
	bool bSuccess = Result->IsSuccessful() && Result->Items.Num() > 0;

	if (bSuccess)
	{
		//Need to use reflection here (which sucks) because the interface is tightly
		//coupled to blackboard. That is unless we use a blackboard state object.
		if (UEnvQueryItemType_Actor::StaticClass() == *Result->ItemType && ActionTaskStateKeyValueReference.ExpectedValueType == EStatePropertyType::Object)
		{
			auto ActorItem = Cast<UEnvQueryItemType_Actor>(ItemTypeCDO);
			CurrentState->SetObject(ActionTaskStateKeyValueReference.StateKeyName, ActorItem->GetActor(Data));
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

void URpaiActionTask_EnvQuery::OnAIMessage(UBrainComponent* BrainComp, const FAIMessage& Message, AAIController* ActionInstigator, URpaiState* CurrentState)
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
