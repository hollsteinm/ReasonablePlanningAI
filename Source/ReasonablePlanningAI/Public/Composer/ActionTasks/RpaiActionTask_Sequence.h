// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/RpaiComposerActionTaskBase.h"
#include "RpaiActionTask_Sequence.generated.h"

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

protected:
	UPROPERTY(EditAnywhere, Category = "Rpai")
	TArray<URpaiComposerActionTaskBase*> Actions;

	virtual void ReceiveStartActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr) override;
	virtual void ReceiveUpdateActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, float DeltaSeconds, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr) override;
	virtual void ReceiveCancelActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr) override;
	virtual void ReceiveCompleteActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr) override;

private:
	void OnActionTaskCompletedOrCancelled(URpaiComposerActionTaskBase* ActionTask, AAIController* ActionInstigator, URpaiState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld);
	TMap<AAIController*, int32> ActiveActionSequenceIndex;
};
