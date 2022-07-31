// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/ReasonablePlanningTypes.h"
#include "Core/ReasonablePlanningActionBase.h"
#include "ReasonablePlanningAction.generated.h"

class UReasonablePlanningWeightBase;
class UReasonablePlanningActionTaskBase;
class UReasonablePlanningStateMutator;
class UReasonablePlanningStateQuery;

/**
 * Composition class for creating composable actions and weight algorithms
 */
UCLASS()
class REASONABLEPLANNINGAI_API UReasonablePlanningAction : public UReasonablePlanningActionBase
{
	GENERATED_BODY()

protected:
	/** Start UReasonablePlanningActionBase*/
	virtual float ReceiveExecutionWeight_Implementation(const UReasonablePlanningState* GivenState) const override;
	virtual void ReceiveStartAction_Implementation(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr) override;
	virtual void ReceiveUpdateAction_Implementation(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, float DeltaSeconds, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr) override;
	virtual void ReceiveCancelAction_Implementation(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr) override;
	virtual void ReceiveCompleteAction_Implementation(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr) override;
    virtual void ReceiveApplyToState_Implementation(UReasonablePlanningState* GivenState) const override;
    virtual bool ReceiveIsApplicable_Implementation(const UReasonablePlanningState* GivenState) const override;
	/** End UReasonablePlanningActionBase*/

    /** The Weight Algorithm to use for determining the effort needed to execute this action. */
	UPROPERTY(Category = "RPAI", EditAnywhere)
	UReasonablePlanningWeightBase* WeightAlgorithm;

    /** The actual task that is ran in game when this action is started. */
	UPROPERTY(Category = "RPAI", EditAnywhere)
	UReasonablePlanningActionTaskBase* ActionTask;
    
    /** Mutators that apply to the state if and only if the action is completed successfully. Used for planning as well as runtime.*/
    UPROPERTY(EditAnywhere, Instanced,Category = "RPAI")
    TArray<UReasonablePlanningStateMutator*> ApplyToStateMutators;
    
    /** The queries to execute to determine if this action is actionable upon the given state. All queries must succeed to for the action to be considered for the given state. Used for planning as well as runtime.*/
    UPROPERTY(EditAnywhere, Instanced,Category = "RPAI")
    UReasonablePlanningStateQuery* IsApplicableQuery;

	/** If set to anything other than None, will attempt to lock a resource on start, cancelling if unable to lock the resource */
	UPROPERTY(Category = "RPAI", EditAnywhere)
	FName LockResourceOnStart;

public:
	FORCEINLINE void SetWeightAlgorithm(UReasonablePlanningWeightBase* NewWeightAlgorithm) { WeightAlgorithm = NewWeightAlgorithm; }

	FORCEINLINE void SetIsApplicableQuery(UReasonablePlanningStateQuery* NewIsApplicableQuery) { IsApplicableQuery = NewIsApplicableQuery; }

	FORCEINLINE void SetStateMutators(TArray<UReasonablePlanningStateMutator*>&& NewMutators) { ApplyToStateMutators = NewMutators; }

	FORCEINLINE void SetAutoLockResourceName(FName NewLockResourceOnStart) { LockResourceOnStart = NewLockResourceOnStart; }

	FORCEINLINE void SetActionTask(UReasonablePlanningActionTaskBase* NewActionTask) { ActionTask = NewActionTask; }

	
};
