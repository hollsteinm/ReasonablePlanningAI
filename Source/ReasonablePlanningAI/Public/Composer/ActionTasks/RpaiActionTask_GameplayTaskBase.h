// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTaskOwnerInterface.h"
#include "Composer/RpaiComposerActionTaskBase.h"
#include "RpaiActionTask_GameplayTaskBase.generated.h"

class UAITask;

/**
 * Action Task that will execute an AITask. Override (Receive)StartActionTask(_Implementation) and use UAITask::NewAITask(...) to create your
 * new task and fill out the parameters. Use StartTask(...) to enqueue the task for execution. Each instance of URpaiActionTask_GameplayTaskBase
 * and all children only support a 1:1 mapping of state to task. Any exsisting tasks will be cancelled if found associated to the given state.
 */
UCLASS(Abstract)
class REASONABLEPLANNINGAI_API URpaiActionTask_GameplayTaskBase : public URpaiComposerActionTaskBase, public IGameplayTaskOwnerInterface
{
	GENERATED_BODY()

public:
	URpaiActionTask_GameplayTaskBase();

	// BEGIN IGameplayTaskOwnerInterface
	virtual UGameplayTasksComponent* GetGameplayTasksComponent(const UGameplayTask& Task) const override;
	virtual AActor* GetGameplayTaskOwner(const UGameplayTask* Task) const override;
	virtual AActor* GetGameplayTaskAvatar(const UGameplayTask* Task) const override;
	virtual uint8 GetGameplayTaskDefaultPriority() const override;
	virtual void OnGameplayTaskInitialized(UGameplayTask& Task) override;
	virtual void OnGameplayTaskDeactivated(UGameplayTask& Task) override;
	// END IGameplayTaskOwnerInterface

protected:
	virtual void ReceiveCancelActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr) override;
	virtual void ReceiveCompleteActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr) override;

	UAITask* GetTaskForState(const URpaiState* CurrentState);
	void StartTask(URpaiState* CurrentState, UAITask* TaskToStart);

private:
	UPROPERTY()
	TMap<const URpaiState*, UAITask*> TaskToState;
};
