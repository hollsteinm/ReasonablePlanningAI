// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/RpaiComposerActionTaskBase.h"
#include "RpaiActionTask_Composite.generated.h"

USTRUCT(BlueprintType)
struct FRpaiCompositeActionTaskEntry
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, Instanced, Category = "Rpai")
	URpaiComposerActionTaskBase* Action;

	/* If true, this action does not matter in the execution of all the actions to determine if the composite action is completed.*/
	UPROPERTY(EditAnywhere, Category = "Rpai")
	bool bIgnoredForCompositeCompletion;

	/* If true, and if `bIgnoredForCompositeCompletion` is true, Cancel will be called on the action task instead of complete upon composite action task completion.*/
	UPROPERTY(EditAnywhere, Category = "Rpai", meta = (EditCondition="bIgnoredForCompositeCompletion"))
	bool bPreferCancelOnCompositeCompletion;

	FRpaiCompositeActionTaskEntry();
	FRpaiCompositeActionTaskEntry(const FRpaiCompositeActionTaskEntry& Other);
};

/**
 * A container action task that executes all Actions listed and does not complete until all
 * actions are completed. Actions are executed as if they are parallel.
 */
UCLASS()
class REASONABLEPLANNINGAI_API URpaiActionTask_Composite : public URpaiComposerActionTaskBase
{
	GENERATED_BODY()

public:
	URpaiActionTask_Composite();

protected:
	/**
	* A list of actions to execute in a batch. All will start at the same time and will complete when all actions have been completed or cancelled unless indicated otherwhise in 'IgnoreIndicesForCompletion'.
	**/
	UPROPERTY(EditAnywhere, Category = "Rpai")
	TArray<FRpaiCompositeActionTaskEntry> ActionEntries;

	virtual void ReceiveStartActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr) override;
	virtual void ReceiveUpdateActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, float DeltaSeconds, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr) override;
	virtual void ReceiveCancelActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr) override;
	virtual void ReceiveCompleteActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr) override;

private:
	void OnActionTaskCompletedOrCancelled(URpaiComposerActionTaskBase* ActionTask, AAIController* ActionInstigator, URpaiState* CurrentState);
	void FlushForController(AAIController* ActionInstigator);
	TMap<AAIController*, TArray<FRpaiCompositeActionTaskEntry>> ActiveActions;
	TMap<AAIController*, TSet<int32>> FlushActionIndices;
};
