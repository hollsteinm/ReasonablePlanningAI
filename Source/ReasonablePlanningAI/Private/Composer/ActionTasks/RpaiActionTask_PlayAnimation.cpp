// Troll Purse. All rights reserved.

#include "Composer/ActionTasks/RpaiActionTask_PlayAnimation.h"
#include "AIController.h"
#include "Core/RpaiTypes.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"

static USkeletalMeshComponent* GetMeshFromController(const AAIController* Controller)
{
	USkeletalMeshComponent* SkelMesh = nullptr;
	ACharacter* const MyCharacter = Cast<ACharacter>(Controller->GetPawn());
	if (MyCharacter)
	{
		SkelMesh = MyCharacter->GetMesh();
	}
	else
	{
		SkelMesh = Controller->GetPawn()->FindComponentByClass<USkeletalMeshComponent>();
	}
	return SkelMesh;
}


URpaiActionTask_PlayAnimation::URpaiActionTask_PlayAnimation()
	: AnimationToPlay(nullptr)
	, bLooping(false)
	, bPersistOnComplete(false)
{
	bCompleteAfterStart = false;
}

void URpaiActionTask_PlayAnimation::ReceiveStartActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	check(AnimationToPlay != nullptr);
	check(ActionInstigator != nullptr);
	USkeletalMeshComponent* SkelMesh = GetMeshFromController(ActionInstigator);

	if (SkelMesh != nullptr)
	{
		auto AnimMode = SkelMesh->GetAnimationMode();
		PreviousAnimationModes.Add(CurrentState, AnimMode);
		SkelMesh->PlayAnimation(AnimationToPlay, bLooping);
		if (!bLooping)
		{
			if (AnimationToPlay->GetPlayLength() > 0)
			{
				FTimerHandle& NewOrExistingHandle = ActiveHandles.FindOrAdd(MoveTemp(CurrentState));
				ActionWorld->GetTimerManager().SetTimer(NewOrExistingHandle, FTimerDelegate::CreateUObject(this, &URpaiActionTask_PlayAnimation::OnAnimationEnd, ActionInstigator, CurrentState, ActionTargetActor, ActionWorld, SkelMesh), AnimationToPlay->GetPlayLength(), false);
			}
			else if (AnimMode == EAnimationMode::AnimationBlueprint && !bPersistOnComplete)
			{
				SkelMesh->SetAnimationMode(AnimMode);
			}
		}
	}
}

void URpaiActionTask_PlayAnimation::ReceiveCompleteActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	if (!bLooping)
	{
		ActiveHandles.Remove(CurrentState);
		PreviousAnimationModes.Remove(CurrentState);
	}
}

void URpaiActionTask_PlayAnimation::ReceiveCancelActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	FTimerHandle MaybeClearTimer;
	if (ActiveHandles.RemoveAndCopyValue(CurrentState, MaybeClearTimer))
	{
		ActionWorld->GetTimerManager().ClearTimer(MaybeClearTimer);
	}

	USkeletalMeshComponent* SkelMesh = GetMeshFromController(ActionInstigator);
	if (SkelMesh != nullptr)
	{
		SkelMesh->Stop();
		EAnimationMode::Type PreviousAnimationMode;
		if (PreviousAnimationModes.RemoveAndCopyValue(CurrentState, PreviousAnimationMode))
		{
			if (PreviousAnimationMode == EAnimationMode::AnimationBlueprint && !bPersistOnComplete)
			{
				SkelMesh->SetAnimationMode(PreviousAnimationMode);
			}
		}
	}
}

void URpaiActionTask_PlayAnimation::OnAnimationEnd(AAIController* ActionInstigator, URpaiState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld, USkeletalMeshComponent* Mesh)
{
	EAnimationMode::Type PreviousAnimationMode;
	if (PreviousAnimationModes.RemoveAndCopyValue(CurrentState, PreviousAnimationMode))
	{
		if (PreviousAnimationMode == EAnimationMode::AnimationBlueprint && !bPersistOnComplete)
		{
			if (Mesh == GetMeshFromController(ActionInstigator))
			{
				Mesh->SetAnimationMode(PreviousAnimationMode);
			}
		}
	}
	if (!bCompleteAfterStart && !bPersistOnComplete)
	{
		CompleteActionTask(ActionInstigator, CurrentState, ActionTargetActor, ActionWorld);
	}
}

