// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/RpaiComposerActionTaskBase.h"
#include "RpaiActionTask_Wait.generated.h"

/**
 * Wait for the time supplied by the given weight key or defined constant. Supports a random deviation to make things interesting.
 */
UCLASS()
class REASONABLEPLANNINGAI_API URpaiActionTask_Wait : public URpaiComposerActionTaskBase
{
	GENERATED_BODY()
public:
	URpaiActionTask_Wait();

protected:
	virtual void ReceiveStartActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr) override;
	virtual void ReceiveCancelActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr) override;
	virtual void ReceiveCompleteActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr) override;

	UPROPERTY(Category = "Rpai", EditAnywhere, meta = (ClampMin = "0.0", UIMin = "0.0"))
	float WaitTimeSeconds;

	UPROPERTY(Category = "Rpai", EditAnywhere, meta = (ClampMin = "0.0", UIMin = "0.0"))
	float RandomDeviation;

private:
	//So we can support a service type action task. Handle won't have valid index set by the time we setup the callback, so use Guid
	TMap<URpaiState*, FTimerHandle> ActiveHandles;
};
