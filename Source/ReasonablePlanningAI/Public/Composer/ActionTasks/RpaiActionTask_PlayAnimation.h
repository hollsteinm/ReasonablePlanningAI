// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/RpaiComposerActionTaskBase.h"
#include "RpaiActionTask_PlayAnimation.generated.h"

/**
 * Plays the defined animation instance and waits for it to complete, timeout, or fire and forget
 */
UCLASS()
class REASONABLEPLANNINGAI_API URpaiActionTask_PlayAnimation : public URpaiComposerActionTaskBase
{
	GENERATED_BODY()

public:
	URpaiActionTask_PlayAnimation();

protected:
	virtual void ReceiveStartActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr) override;
	virtual void ReceiveCancelActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr) override;
	virtual void ReceiveCompleteActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr) override;

	UPROPERTY(Category = "Rpai", EditAnywhere)
	UAnimationAsset* AnimationToPlay;

	UPROPERTY(Category = "Rpai", EditAnywhere)
	bool bLooping;

	UPROPERTY(Category = "Rpai", EditAnywhere)
	bool bPersistOnComplete;

private:
	//So we can support a service type action task. Handle won't have valid index set by the time we setup the callback, so use Guid
	TMap<URpaiState*, FTimerHandle> ActiveHandles;
	TMap<URpaiState*, EAnimationMode::Type> PreviousAnimationModes;

	void OnAnimationEnd(AAIController* ActionInstigator, URpaiState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld, USkeletalMeshComponent* Mesh);
};
