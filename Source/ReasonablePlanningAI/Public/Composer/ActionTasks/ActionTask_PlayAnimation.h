// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/ReasonablePlanningActionTaskBase.h"
#include "ActionTask_PlayAnimation.generated.h"

/**
 * Plays the defined animation instance and waits for it to complete, timeout, or fire and forget
 */
UCLASS()
class REASONABLEPLANNINGAI_API UActionTask_PlayAnimation : public UReasonablePlanningActionTaskBase
{
	GENERATED_BODY()

public:
	UActionTask_PlayAnimation();

protected:
	virtual void ReceiveStartActionTask_Implementation(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr) override;
	virtual void ReceiveCancelActionTask_Implementation(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr) override;
	virtual void ReceiveCompleteActionTask_Implementation(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr) override;

	UPROPERTY(Category = "RPAI", EditAnywhere)
	UAnimationAsset* AnimationToPlay;

	UPROPERTY(Category = "RPAI", EditAnywhere)
	bool bLooping;

private:
	//So we can support a service type action task. Handle won't have valid index set by the time we setup the callback, so use Guid
	TMap<UReasonablePlanningState*, FTimerHandle> ActiveHandles;

	void OnAnimationEnd(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld, USkeletalMeshComponent* Mesh, EAnimationMode::Type PreviousAnimationMode);
};
