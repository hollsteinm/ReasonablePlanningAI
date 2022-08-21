// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/RpaiComposerActionTaskBase.h"
#include "RpaiActionTask_Composite.generated.h"

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
	UPROPERTY(EditAnywhere, Instanced, Category = "Rpai")
	TArray<URpaiComposerActionTaskBase*> Actions;

	virtual void ReceiveStartActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr) override;
	virtual void ReceiveUpdateActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, float DeltaSeconds, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr) override;
	virtual void ReceiveCancelActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr) override;
	virtual void ReceiveCompleteActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr) override;

private:
	void OnActionTaskCompletedOrCancelled(URpaiComposerActionTaskBase* ActionTask, AAIController* ActionInstigator, URpaiState* CurrentState);
	void FlushForController(AAIController* ActionInstigator);
	TMap<AAIController*, TArray<URpaiComposerActionTaskBase*>> ActiveActions;
	TMap<AAIController*, TSet<int32>> FlushActionIndices;
};
