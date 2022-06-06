// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ReasonablePlanningTypes.h"
#include "ReasonablePlanningActionBase.h"
#include "ReasonablePlanningAction.generated.h"

class UReasonablePlanningWeightBase;
class UReasonablePlanningActionTaskBase;

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
	virtual void ReceiveCompleteAction_Implementation(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr) override;	/** End UReasonablePlanningActionBase*/
	/** End UReasonablePlanningActionBase*/

	UPROPERTY(Category = "ReasonablePlanningAI", EditAnywhere)
	UReasonablePlanningWeightBase* WeightAlgorithm;

	UPROPERTY(Category = "ReasonablePlanningAI", EditAnywhere)
	UReasonablePlanningActionTaskBase* ActionTask;

	/** If set to anything other than None, will attempt to lock a resource on start, cancelling if unable to lock the resource */
	UPROPERTY(Category = "ReasonablePlanningAI", EditAnywhere)
	FName LockResourceOnStart;
};
