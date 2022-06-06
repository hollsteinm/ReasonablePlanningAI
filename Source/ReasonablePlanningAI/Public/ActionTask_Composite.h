// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ReasonablePlanningActionTaskBase.h"
#include "ActionTask_Composite.generated.h"

/**
 * A container action task that executes all Actions listed and does not complete until all
 * actions are completed.
 */
UCLASS()
class REASONABLEPLANNINGAI_API UActionTask_Composite : public UReasonablePlanningActionTaskBase
{
	GENERATED_BODY()

public:
	UActionTask_Composite();

protected:
	UPROPERTY(EditAnywhere, Instanced,Category="ReasonablePlanningAI")
	TArray<UReasonablePlanningActionTaskBase*> Actions;

	virtual void ReceiveStartActionTask_Implementation(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr) override;
	virtual void ReceiveUpdateActionTask_Implementation(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, float DeltaSeconds, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr) override;
	virtual void ReceiveCancelActionTask_Implementation(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr) override;
	virtual void ReceiveCompleteActionTask_Implementation(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr) override;

private:
	void OnActionTaskCompletedOrCancelled(UReasonablePlanningActionTaskBase* ActionTask, AAIController* ActionInstigator, UReasonablePlanningState* CurrentState);
	void FlushForController(AAIController* ActionInstigator);
	TMap<AAIController*, TArray<UReasonablePlanningActionTaskBase*>> ActiveActions;
	TMap<AAIController*, TSet<int32>> FlushActionIndices;
};
