// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/RpaiComposerActionTaskBase.h"
#include "RpaiActionTask_Sequence.generated.h"

USTRUCT(BlueprintType)
struct REASONABLEPLANNINGAI_API FActionTaskSequence
{
	GENERATED_BODY()

	FActionTaskSequence();
	~FActionTaskSequence();

	int32 ActiveActionTaskSequenceIndex;
	FRpaiMemoryStruct ActiveActionTaskMemorySlice;
	bool bCancelledFromSequence;
};

/**
 * A container action task that executes all Actions listed and does not complete until all
 * actions are completed. Actions are executed in the order defined, only advancing after another has completed.
 */
UCLASS()
class REASONABLEPLANNINGAI_API URpaiActionTask_Sequence : public URpaiComposerActionTaskBase
{
	GENERATED_BODY()
public:
	URpaiActionTask_Sequence();

	/**
	* Adds an action task to this sequence of actions. Not safe to call during execution.
	**/
	UFUNCTION(BlueprintCallable, Category = "Rpai")
	void AddActionTaskToSequence(URpaiComposerActionTaskBase* NewAction);

	/**
	* Removes all occurences of an action task to this sequence of actions. Not safe to call during execution.
	**/
	UFUNCTION(BlueprintCallable, Category = "Rpai")
	void RemoveActionTaskFromSequence(const URpaiComposerActionTaskBase* ActionToRemove);

	UFUNCTION(BlueprintPure, Category = "Rpai")
	const URpaiComposerActionTaskBase* ViewTaskFromSequence(int32 Index) const;

protected:
	UPROPERTY(EditAnywhere, Category = "Rpai")
	TArray<URpaiComposerActionTaskBase*> Actions;

	virtual void ReceiveStartActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr) override;
	virtual void ReceiveUpdateActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, float DeltaSeconds, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr) override;
	virtual void ReceiveCancelActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr, bool bCancelShouldExitPlan = true) override;

private:
	void OnActionTaskCompletedOrCancelled(URpaiComposerActionTaskBase* ActionTask, AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld);
	void OnSequenceActionTaskCancelled(URpaiComposerActionTaskBase* ActionTask, AAIController* ActionInstigator, URpaiState* CurrentState, bool bCancelShouldExitPlan, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld);

	FRpaiMemory SequenceMemoryPool;
};
