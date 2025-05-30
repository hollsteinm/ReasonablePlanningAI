// Radaway Software LLC. 2025. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/RpaiComposerActionTask.h"
#include "RpaiActionTask_RotateTo.generated.h"

/**
 * Rotate to face the given state key.
 */
UCLASS(config = Game)
class REASONABLEPLANNINGAI_API URpaiActionTask_RotateTo : public URpaiComposerActionTask
{
	GENERATED_BODY()

public:
	URpaiActionTask_RotateTo();

private:
	/** cached Precision tangent value */
	float PrecisionDot;

protected:
	virtual void ReceiveStartActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr) override;
	virtual void ReceiveUpdateActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, float DeltaSeconds, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr) override;
	virtual void ReceiveCancelActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr, bool bCancelShouldExitPlan = true) override;

	/** Success condition precision in degrees */
	UPROPERTY(config, EditAnywhere, meta = (ClampMin = "0.0"), Category = "Rpai")
	float Precision;

	FORCEINLINE float GetPrecisionDot() const { return PrecisionDot; }

public:
	virtual void PostInitProperties() override;
	virtual void PostLoad() override;
};
