// Troll Purse. All rights reserved.

#include "Composer/ActionTasks/RpaiActionTask_PlayAnimation.h"
#include "AIController.h"
#include "Core/RpaiTypes.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"

FActionTaskPlayAnimationMemory::FActionTaskPlayAnimationMemory()
	: AnimationTimerHandle()
	, PreviousAnimationMode(EAnimationMode::Type::AnimationCustomMode)
	, CachedMesh(nullptr)
{

}

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
	ActionTaskMemoryStructType = FActionTaskPlayAnimationMemory::StaticStruct();
}

void URpaiActionTask_PlayAnimation::ReceiveStartActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	check(AnimationToPlay != nullptr);
	check(ActionInstigator != nullptr);
	USkeletalMeshComponent* SkelMesh = GetMeshFromController(ActionInstigator);

	if (SkelMesh != nullptr)
	{
		FActionTaskPlayAnimationMemory* Memory = ActionMemory.Get<FActionTaskPlayAnimationMemory>();
		auto AnimMode = SkelMesh->GetAnimationMode();
		
		Memory->CachedMesh = SkelMesh;
		Memory->PreviousAnimationMode = AnimMode;

		SkelMesh->PlayAnimation(AnimationToPlay, bLooping);
		if (!bLooping)
		{
			if (AnimationToPlay->GetPlayLength() > 0)
			{
				ActionWorld->GetTimerManager().SetTimer(Memory->AnimationTimerHandle, FTimerDelegate::CreateUObject(this, &URpaiActionTask_PlayAnimation::OnAnimationEnd, ActionInstigator, CurrentState, ActionMemory, ActionTargetActor, ActionWorld, SkelMesh), AnimationToPlay->GetPlayLength(), false);
			}
			else if (AnimMode == EAnimationMode::AnimationBlueprint && !bPersistOnComplete)
			{
				SkelMesh->SetAnimationMode(AnimMode);
			}
		}
	}
}

void URpaiActionTask_PlayAnimation::ReceiveCancelActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	FActionTaskPlayAnimationMemory* Memory = ActionMemory.Get<FActionTaskPlayAnimationMemory>();
	ActionWorld->GetTimerManager().ClearTimer(Memory->AnimationTimerHandle);

	USkeletalMeshComponent* SkelMesh = Memory->CachedMesh;
	if (SkelMesh != nullptr)
	{
		SkelMesh->Stop();
		if (Memory->PreviousAnimationMode == EAnimationMode::AnimationBlueprint && !bPersistOnComplete)
		{
			SkelMesh->SetAnimationMode(Memory->PreviousAnimationMode);
		}
	}
}

void URpaiActionTask_PlayAnimation::OnAnimationEnd(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld, USkeletalMeshComponent* Mesh)
{
	FActionTaskPlayAnimationMemory* Memory = ActionMemory.Get<FActionTaskPlayAnimationMemory>();
	if (Memory->PreviousAnimationMode == EAnimationMode::AnimationBlueprint && !bPersistOnComplete)
	{
		if (Mesh == Memory->CachedMesh)
		{
			Mesh->SetAnimationMode(Memory->PreviousAnimationMode);
		}
	}

	if (!bCompleteAfterStart && !bPersistOnComplete)
	{
		CompleteActionTask(ActionInstigator, CurrentState, ActionMemory, ActionTargetActor, ActionWorld);
	}
}

