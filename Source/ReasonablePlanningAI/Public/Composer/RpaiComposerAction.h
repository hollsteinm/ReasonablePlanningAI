// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/RpaiTypes.h"
#include "Core/RpaiActionBase.h"
#include "RpaiComposerAction.generated.h"

class URpaiComposerWeightBase;
class URpaiComposerActionTaskBase;
class URpaiComposerStateMutator;
class URpaiComposerStateQuery;

/**
 * Composition class for creating composable actions and weight algorithms
 */
UCLASS()
class REASONABLEPLANNINGAI_API URpaiComposerAction : public URpaiActionBase
{
	GENERATED_BODY()

public:
	virtual FRpaiMemoryStruct AllocateMemorySlice(FRpaiMemory& FromMemory) const;

private:
	void HandleActionTaskCompleted(URpaiComposerActionTaskBase* CompletedActionTask, AAIController* ActionInstigator, URpaiState* State, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld);

protected:
	/** Start URpaiActionBase*/
	virtual float ReceiveExecutionWeight_Implementation(const URpaiState* GivenState) const override;
	virtual void ReceiveStartAction_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr) override;
	virtual void ReceiveUpdateAction_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, float DeltaSeconds, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr) override;
	virtual void ReceiveCancelAction_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr) override;
	virtual void ReceiveCompleteAction_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr) override;
    virtual void ReceiveApplyToState_Implementation(URpaiState* GivenState) const override;
    virtual bool ReceiveIsApplicable_Implementation(const URpaiState* GivenState) const override;
	/** End URpaiActionBase*/

    /** The Weight Algorithm to use for determining the effort needed to execute this action. */
	UPROPERTY(Category = "Rpai", EditAnywhere)
	URpaiComposerWeightBase* WeightAlgorithm;

    /** The actual task that is ran in game when this action is started. */
	UPROPERTY(Category = "Rpai", EditAnywhere)
	URpaiComposerActionTaskBase* ActionTask;
    
    /** Mutators that apply to the state if and only if the action is completed successfully. Used for planning as well as runtime.*/
    UPROPERTY(EditAnywhere, Instanced,Category = "Rpai")
    TArray<URpaiComposerStateMutator*> ApplyToStateMutators;
    
    /** The queries to execute to determine if this action is actionable upon the given state. All queries must succeed to for the action to be considered for the given state. Used for planning as well as runtime.*/
    UPROPERTY(EditAnywhere, Instanced,Category = "Rpai")
    URpaiComposerStateQuery* IsApplicableQuery;

	/** If set to anything other than None, will attempt to lock a resource on start, cancelling if unable to lock the resource */
	UPROPERTY(Category = "Rpai", EditAnywhere)
	FName LockResourceOnStart;

public:
	FORCEINLINE void SetWeightAlgorithm(URpaiComposerWeightBase* NewWeightAlgorithm) { WeightAlgorithm = NewWeightAlgorithm; }

	FORCEINLINE void SetIsApplicableQuery(URpaiComposerStateQuery* NewIsApplicableQuery) { IsApplicableQuery = NewIsApplicableQuery; }

	FORCEINLINE void SetStateMutators(TArray<URpaiComposerStateMutator*>&& NewMutators) { ApplyToStateMutators = NewMutators; }

	FORCEINLINE void SetAutoLockResourceName(FName NewLockResourceOnStart) { LockResourceOnStart = NewLockResourceOnStart; }

	FORCEINLINE void SetActionTask(URpaiComposerActionTaskBase* NewActionTask) { ActionTask = NewActionTask; }

	
};
