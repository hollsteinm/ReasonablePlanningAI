// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ReasonablePlanningActionTask.h"
#include "ActionTask_RotateTo.generated.h"

/**
 * Rotate to face the given state key.
 */
UCLASS(config = Game)
class REASONABLEPLANNINGAI_API UActionTask_RotateTo : public UReasonablePlanningActionTask
{
	GENERATED_BODY()

public:
	UActionTask_RotateTo();

private:
	/** cached Precision tangent value */
	float PrecisionDot;

protected:
	virtual void ReceiveStartActionTask_Implementation(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr) override;
	virtual void ReceiveUpdateActionTask_Implementation(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, float DeltaSeconds, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr) override;
	virtual void ReceiveCancelActionTask_Implementation(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr) override;

	/** Success condition precision in degrees */
	UPROPERTY(config, Category = Node, EditAnywhere, meta = (ClampMin = "0.0"))
	float Precision;

	FORCEINLINE float GetPrecisionDot() const { return PrecisionDot; }

public:
	virtual void PostInitProperties() override;
	virtual void PostLoad() override;
};
