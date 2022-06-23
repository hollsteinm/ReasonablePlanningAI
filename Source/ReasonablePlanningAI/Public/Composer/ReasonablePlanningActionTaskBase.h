// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ReasonablePlanningActionTaskBase.generated.h"

class UReasonablePlanningActionTaskBase;
class UReasonablePlanningState;
class AAIController;
class AActor;
class UWorld;

DECLARE_EVENT_ThreeParams(UReasonablePlanningState, FActionTaskStarted, UReasonablePlanningActionTaskBase*, AAIController*, UReasonablePlanningState*);
DECLARE_EVENT_ThreeParams(UReasonablePlanningState, FActionTaskCompleted, UReasonablePlanningActionTaskBase*, AAIController*, UReasonablePlanningState*);
DECLARE_EVENT_ThreeParams(UReasonablePlanningState, FActionTaskCancelled, UReasonablePlanningActionTaskBase*, AAIController*, UReasonablePlanningState*);
DECLARE_EVENT_FourParams(UReasonablePlanningState, FActionTaskUpdated, UReasonablePlanningActionTaskBase*, AAIController*, UReasonablePlanningState*, float);

/**
 * A class used as a helper for composition of actions that implements only the
 * action logic of a given action
 */
UCLASS(BlueprintType, Blueprintable, Abstract, EditInlineNew, DefaultToInstanced)
class REASONABLEPLANNINGAI_API UReasonablePlanningActionTaskBase : public UObject
{
	GENERATED_BODY()

private:
	FActionTaskStarted ActionTaskStartedEvent;
	FActionTaskCompleted ActionTaskCompletedEvent;
	FActionTaskCancelled ActionTaskCancelledEvent;
	FActionTaskUpdated ActionTaskUpdatedEvent;
	
public:
	UReasonablePlanningActionTaskBase();

	/**
	* The initial logic to run when this action is entered. For actions that do not run over time, this is the appropriate action to call.
	*/
	UFUNCTION(BlueprintCallable, Category = "RPAI")
	void StartActionTask(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr);

	/**
	* Subsequent calls to this action state after StartAction has been called and before CancelAction or CompleteAction have been called. Will not execute
	* if IsActionComplete returns true after invoking StartAction.
	*/
	UFUNCTION(BlueprintCallable, Category = "RPAI")
	void UpdateActionTask(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, float DeltaSeconds, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr);

	/**
	* Immediately terminate the current action and apply any state changes needed to revert this action (if desired).
	*/
	UFUNCTION(BlueprintCallable, Category = "RPAI")
	void CancelActionTask(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr);

	/**
	* Called immediately after IsActionComplete returns true. Use this to update the state
	*/
	UFUNCTION(BlueprintCallable, Category = "RPAI")
	void CompleteActionTask(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr);

	/**
	* Event indicating that the action task has started.
	*/
	FActionTaskStarted& OnActionTaskStarted() { return ActionTaskStartedEvent; }

	/**
	* Event indicating that the action task has completed.
	*/
	FActionTaskCompleted& OnActionTaskComplete() { return ActionTaskCompletedEvent; }

	/**
	* Event indicating that the action task has been cancelled.
	*/
	FActionTaskCancelled& OnActionTaskCancelled() { return ActionTaskCancelledEvent; }

	/**
	* Event indicating that the action task has updated.
	*/
	FActionTaskUpdated& OnActionTaskUpdated() { return ActionTaskUpdatedEvent; }

protected:
	/**
	* Override this to implement StartAction
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "ResoanablePlanning")
	void ReceiveStartActionTask(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr);
	virtual void ReceiveStartActionTask_Implementation(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr);

	/**
	* Override this to implement UpdateAction
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "RPAI")
	void ReceiveUpdateActionTask(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, float DeltaSeconds, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr);
	virtual void ReceiveUpdateActionTask_Implementation(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, float DeltaSeconds, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr);

	/**
	* Override this to implement CancelAction
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "RPAI")
	void ReceiveCancelActionTask(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr);
	virtual void ReceiveCancelActionTask_Implementation(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr);

	/**
	* Override this to implement CompleteAction
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "RPAI")
	void ReceiveCompleteActionTask(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr);
	virtual void ReceiveCompleteActionTask_Implementation(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr);

	UPROPERTY(EditAnywhere, Category = "RPAI")
	bool bCompleteAfterStart;
};
