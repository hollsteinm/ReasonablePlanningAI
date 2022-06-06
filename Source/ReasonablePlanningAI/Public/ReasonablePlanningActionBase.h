// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ReasonablePlanningActionBase.generated.h"

class UReasonablePlanningActionBase;
class UReasonablePlanningState;
class AAIController;
class AActor;
class UWorld;

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_ThreeParams(FActionStartedSignature, UReasonablePlanningActionBase, OnActionStarted, UReasonablePlanningActionBase*, StartedAction, AAIController*, ActionInstigator, UReasonablePlanningState*, CurrentState);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_ThreeParams(FActionCompletedSignature, UReasonablePlanningActionBase, OnActionComplete, UReasonablePlanningActionBase*, CompletedAction, AAIController*, ActionInstigator, UReasonablePlanningState*, CurrentState);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_ThreeParams(FActionCancelledSignature, UReasonablePlanningActionBase, OnActionCancelled, UReasonablePlanningActionBase*, CancelledAction, AAIController*, ActionInstigator, UReasonablePlanningState*, CurrentState);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_FourParams(FActionUpdatedSignature, UReasonablePlanningActionBase, OnActionUpdated, UReasonablePlanningActionBase*, UpdatedAction, AAIController*, ActionInstigator, UReasonablePlanningState*, CurrentState, float, DeltaSeconds);

/**
 * An action is the granular execution of a single verb to help accomplish
 * a desired goal state. It is the partial execution needed to reach a given
 * state and applys changes to the current state during execution or on
 * completion of the action. Examples of actions are playing an animation or
 * navigating to a location.
 */
UCLASS(BlueprintType, Blueprintable, Abstract, EditInlineNew, DefaultToInstanced)
class REASONABLEPLANNINGAI_API UReasonablePlanningActionBase : public UObject
{
	GENERATED_BODY()

public:
	/**
	* Applies to the state the values if this action is completed.
	* If there are dynamic values (i.e. values that range over time or are more than simple booleans)
	* using hueristics is a good way to go. Used in action planning
	*/
	UFUNCTION(BlueprintCallable, Category = "ReasonablePlanning")
	void ApplyToState(UReasonablePlanningState* GivenState) const;

	/**
	* Determine if this action can even be executed on. Used in action planning.
	*/
	UFUNCTION(BlueprintPure, Category = "ReasonablePlanning")
	bool IsApplicable(const UReasonablePlanningState* GivenState) const;

	/**
	* With the given state, how expensive is it to execute this action? Higher values
	* mean more expensive and less likely to result in a shorter path to achieving a goal.
	*/
	UFUNCTION(BlueprintPure, Category = "ReasonablePlanning")
	float ExecutionWeight(const UReasonablePlanningState* GivenState) const;

	/**
	* The initial logic to run when this action is entered. For actions that do not run over time, this is the appropriate action to call.
	*/
	UFUNCTION(BlueprintCallable, Category = "ReasonablePlanning")
	void StartAction(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr);

	/**
	* Subsequent calls to this action state after StartAction has been called and before CancelAction or CompleteAction have been called. Will not execute
	* if IsActionComplete returns true after invoking StartAction.
	*/
	UFUNCTION(BlueprintCallable, Category = "ReasonablePlanning")
	void UpdateAction(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, float DeltaSeconds, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr);

	/**
	* Immediately terminate the current action and apply any state changes needed to revert this action (if desired).
	*/
	UFUNCTION(BlueprintCallable, Category = "ReasonablePlanning")
	void CancelAction(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr);

	/**
	* Called immediately after IsActionComplete returns true. Use this to update the state
	*/
	UFUNCTION(BlueprintCallable, Category = "ReasonablePlanning")
	void CompleteAction(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr);

	/**
	* Determines if the current action should be exited successfully and call CompleteAction
	*/
	UFUNCTION(BlueprintPure, Category = "ReasonablePlanning")
	bool IsActionComplete(const AAIController* ActionInstigator, const UReasonablePlanningState* CurrentState, const AActor* ActionTargetActor = nullptr, const UWorld* ActionWorld = nullptr) const;

	/**
	* Event indicating that the action has started.
	*/
	UPROPERTY(BlueprintAssignable, Category = "ReasonablePlanning")
	FActionStartedSignature OnActionStarted;

	/**
	* Event indicating that the action has completed.
	*/
	UPROPERTY(BlueprintAssignable, Category = "ReasonablePlanning")
	FActionCompletedSignature OnActionComplete;

	/**
	* Event indicating that the action has been cancelled.
	*/
	UPROPERTY(BlueprintAssignable, Category = "ReasonablePlanning")
	FActionCancelledSignature OnActionCancelled;

	/**
	* Event indicating that the action has updated.
	*/
	UPROPERTY(BlueprintAssignable, Category = "ReasonablePlanning")
	FActionUpdatedSignature OnActionUpdated;

protected:
	/**
	* Override this to implement ApplyToState
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "ResoanablePlanning")
	void ReceiveApplyToState(UReasonablePlanningState* GivenState) const;
	virtual void ReceiveApplyToState_Implementation(UReasonablePlanningState* GivenState) const;

	/**
	* Override this to implement IsApplicable
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "ResoanablePlanning")
	bool ReceiveIsApplicable(const UReasonablePlanningState* GivenState) const;
	virtual bool ReceiveIsApplicable_Implementation(const UReasonablePlanningState* GivenState) const;

	/**
	* Override this to implement ExecutionWeight
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "ResoanablePlanning")
	float ReceiveExecutionWeight(const UReasonablePlanningState* GivenState) const;
	virtual float ReceiveExecutionWeight_Implementation(const UReasonablePlanningState* GivenState) const;

	/**
	* Override this to implement StartAction
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "ResoanablePlanning")
	void ReceiveStartAction(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr);
	virtual void ReceiveStartAction_Implementation(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr);

	/**
	* Override this to implement UpdateAction
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "ReasonablePlanning")
	void ReceiveUpdateAction(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, float DeltaSeconds, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr);
	virtual void ReceiveUpdateAction_Implementation(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, float DeltaSeconds, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr);

	/**
	* Override this to implement CancelAction
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "ReasonablePlanning")
	void ReceiveCancelAction(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr);
	virtual void ReceiveCancelAction_Implementation(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr);

	/**
	* Override this to implement CompleteAction
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "ReasonablePlanning")
	void ReceiveCompleteAction(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr);
	virtual void ReceiveCompleteAction_Implementation(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr);

	/**
	* Override this to implement IsActionComplete
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "ResoanablePlanning")
	bool ReceiveIsActionComplete(const AAIController* ActionInstigator, const UReasonablePlanningState* CurrentState, const AActor* ActionTargetActor = nullptr, const UWorld* ActionWorld = nullptr) const;
	virtual bool ReceiveIsActionComplete_Implementation(const AAIController* ActionInstigator, const UReasonablePlanningState* CurrentState, const AActor* ActionTargetActor = nullptr, const UWorld* ActionWorld = nullptr) const;
};
