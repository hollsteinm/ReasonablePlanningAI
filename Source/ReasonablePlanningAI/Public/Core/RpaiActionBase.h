// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/RpaiTypes.h"
#include "RpaiActionBase.generated.h"

class URpaiActionBase;
class URpaiState;
class AAIController;
class AActor;
class UWorld;

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_ThreeParams(FActionStartedSignature, URpaiActionBase, OnActionStarted, URpaiActionBase*, StartedAction, AAIController*, ActionInstigator, URpaiState*, CurrentState);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_ThreeParams(FActionCompletedSignature, URpaiActionBase, OnActionComplete, URpaiActionBase*, CompletedAction, AAIController*, ActionInstigator, URpaiState*, CurrentState);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_ThreeParams(FActionCancelledSignature, URpaiActionBase, OnActionCancelled, URpaiActionBase*, CancelledAction, AAIController*, ActionInstigator, URpaiState*, CurrentState);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_FourParams(FActionUpdatedSignature, URpaiActionBase, OnActionUpdated, URpaiActionBase*, UpdatedAction, AAIController*, ActionInstigator, URpaiState*, CurrentState, float, DeltaSeconds);

/**
 * An action is the granular execution of a single verb to help accomplish
 * a desired goal state. It is the partial execution needed to reach a given
 * state and applys changes to the current state during execution or on
 * completion of the action. Examples of actions are playing an animation or
 * navigating to a location.
 */
UCLASS(BlueprintType, Blueprintable, Abstract, EditInlineNew, DefaultToInstanced)
class REASONABLEPLANNINGAI_API URpaiActionBase : public UObject
{
	GENERATED_BODY()

public:
	URpaiActionBase();

	/**
	* Applies to the state the values if this action is completed.
	* If there are dynamic values (i.e. values that range over time or are more than simple booleans)
	* using hueristics is a good way to go. Used in action planning
	*/
	UFUNCTION(BlueprintCallable, Category = "Rpai")
	void ApplyToState(URpaiState* GivenState) const;

	/**
	* Determine if this action can even be executed on. Used in action planning.
	*/
	UFUNCTION(BlueprintPure, Category = "Rpai")
	bool IsApplicable(const URpaiState* GivenState) const;

	/**
	* With the given state, how expensive is it to execute this action? Higher values
	* mean more expensive and less likely to result in a shorter path to achieving a goal.
	*/
	UFUNCTION(BlueprintPure, Category = "Rpai")
	float ExecutionWeight(const URpaiState* GivenState) const;

	/**
	* The initial logic to run when this action is entered. For actions that do not run over time, this is the appropriate action to call.
	*/
	UFUNCTION(BlueprintCallable, Category = "Rpai")
	void StartAction(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr);

	/**
	* Subsequent calls to this action state after StartAction has been called and before CancelAction or CompleteAction have been called. Will not execute
	* if IsActionComplete returns true after invoking StartAction.
	*/
	UFUNCTION(BlueprintCallable, Category = "Rpai")
	void UpdateAction(AAIController* ActionInstigator, URpaiState* CurrentState, float DeltaSeconds, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr);

	/**
	* Immediately terminate the current action and apply any state changes needed to revert this action (if desired).
	*/
	UFUNCTION(BlueprintCallable, Category = "Rpai")
	void CancelAction(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr);

	/**
	* Called immediately after IsActionComplete returns true. Use this to update the state
	*/
	UFUNCTION(BlueprintCallable, Category = "Rpai")
	void CompleteAction(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr);

	/**
	* Determines if the current action should be exited successfully and call CompleteAction
	*/
	UFUNCTION(BlueprintPure, Category = "Rpai")
	bool IsActionComplete(const AAIController* ActionInstigator, const URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, const AActor* ActionTargetActor = nullptr, const UWorld* ActionWorld = nullptr) const;

	/**
	* Event indicating that the action has started.
	*/
	UPROPERTY(BlueprintAssignable, Category = "Rpai")
	FActionStartedSignature OnActionStarted;

	/**
	* Event indicating that the action has completed.
	*/
	UPROPERTY(BlueprintAssignable, Category = "Rpai")
	FActionCompletedSignature OnActionComplete;

	/**
	* Event indicating that the action has been cancelled.
	*/
	UPROPERTY(BlueprintAssignable, Category = "Rpai")
	FActionCancelledSignature OnActionCancelled;

	/**
	* Event indicating that the action has updated.
	*/
	UPROPERTY(BlueprintAssignable, Category = "Rpai")
	FActionUpdatedSignature OnActionUpdated;
    
private:
    UPROPERTY(EditAnywhere, Category = "Rpai")
    FString ActionName;
 
public:
    UFUNCTION(BlueprintPure, Category = "Rpai")
    FString GetActionName() const;

	UFUNCTION(BlueprintPure, Category = "Rpai")
	virtual FRpaiMemoryStruct AllocateMemorySlice(FRpaiMemory& FromMemory) const;

protected:
	/**
	* Struct used for runtime memory for each executing instance.
	**/
	UPROPERTY(EditDefaultsOnly, Category = "Rpai")
	UScriptStruct* ActionMemoryStructType;

	/**
	* Override this to implement ApplyToState
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "Rpai")
	void ReceiveApplyToState(URpaiState* GivenState) const;
	virtual void ReceiveApplyToState_Implementation(URpaiState* GivenState) const;

	/**
	* Override this to implement IsApplicable
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Rpai")
	bool ReceiveIsApplicable(const URpaiState* GivenState) const;
	virtual bool ReceiveIsApplicable_Implementation(const URpaiState* GivenState) const;

	/**
	* Override this to implement ExecutionWeight
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Rpai")
	float ReceiveExecutionWeight(const URpaiState* GivenState) const;
	virtual float ReceiveExecutionWeight_Implementation(const URpaiState* GivenState) const;

	/**
	* Override this to implement StartAction
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "Rpai")
	void ReceiveStartAction(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr);
	virtual void ReceiveStartAction_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr);

	/**
	* Override this to implement UpdateAction
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "Rpai")
	void ReceiveUpdateAction(AAIController* ActionInstigator, URpaiState* CurrentState, float DeltaSeconds, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr);
	virtual void ReceiveUpdateAction_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, float DeltaSeconds, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr);

	/**
	* Override this to implement CancelAction
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "Rpai")
	void ReceiveCancelAction(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr);
	virtual void ReceiveCancelAction_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr);

	/**
	* Override this to implement CompleteAction
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "Rpai")
	void ReceiveCompleteAction(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr);
	virtual void ReceiveCompleteAction_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr);

	/**
	* Override this to implement IsActionComplete
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Rpai")
	bool ReceiveIsActionComplete(const AAIController* ActionInstigator, const URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, const AActor* ActionTargetActor = nullptr, const UWorld* ActionWorld = nullptr) const;
	virtual bool ReceiveIsActionComplete_Implementation(const AAIController* ActionInstigator, const URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, const AActor* ActionTargetActor = nullptr, const UWorld* ActionWorld = nullptr) const;
};
