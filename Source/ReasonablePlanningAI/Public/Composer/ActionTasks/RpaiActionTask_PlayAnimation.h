// Radaway Software LLC. 2025. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/RpaiComposerActionTaskBase.h"
#include "RpaiActionTask_PlayAnimation.generated.h"

USTRUCT(BlueprintType)
struct REASONABLEPLANNINGAI_API FActionTaskPlayAnimationMemory
{
	GENERATED_BODY()

	FActionTaskPlayAnimationMemory();

	FTimerHandle AnimationTimerHandle;
	EAnimationMode::Type PreviousAnimationMode;
	USkeletalMeshComponent* CachedMesh;
};

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
	virtual void ReceiveStartActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr) override;
	virtual void ReceiveCancelActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr, bool bCancelShouldExitPlan = true) override;

	UPROPERTY(Category = "Rpai", EditAnywhere)
	UAnimationAsset* AnimationToPlay;

	UPROPERTY(Category = "Rpai", EditAnywhere)
	bool bLooping;

	UPROPERTY(Category = "Rpai", EditAnywhere)
	bool bPersistOnComplete;

private:
	void OnAnimationEnd(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld, USkeletalMeshComponent* Mesh);
};
