// Troll Purse. All rights reserved.

#include "Composer/ActionTasks/ActionTask_PlayAnimation.h"
#include "AIController.h"
#include "Core/ReasonablePlanningTypes.h"
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


UActionTask_PlayAnimation::UActionTask_PlayAnimation()
	: AnimationToPlay(nullptr)
	, bLooping(false)
{
	bCompleteAfterStart = false;
}

void UActionTask_PlayAnimation::ReceiveStartActionTask_Implementation(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	check(AnimationToPlay != nullptr);
	check(ActionInstigator != nullptr);
	USkeletalMeshComponent* SkelMesh = GetMeshFromController(ActionInstigator);

	if (SkelMesh != nullptr)
	{
		auto AnimMode = SkelMesh->GetAnimationMode();
		SkelMesh->PlayAnimation(AnimationToPlay, bLooping);
		if (AnimationToPlay->GetMaxCurrentTime() > 0 && !bLooping)
		{
			FTimerHandle& NewOrExistingHandle = ActiveHandles.FindOrAdd(MoveTemp(CurrentState));
			ActionWorld->GetTimerManager().SetTimer(NewOrExistingHandle, FTimerDelegate::CreateUObject(this, &UActionTask_PlayAnimation::OnAnimationEnd, ActionInstigator, CurrentState, ActionTargetActor, ActionWorld, SkelMesh, AnimMode), AnimationToPlay->GetMaxCurrentTime(), false);
		}
		else if(AnimMode == EAnimationMode::AnimationBlueprint)
		{
			SkelMesh->SetAnimationMode(AnimMode);
		}
	}
}

void UActionTask_PlayAnimation::ReceiveCompleteActionTask_Implementation(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	ActiveHandles.Remove(CurrentState);
}

void UActionTask_PlayAnimation::ReceiveCancelActionTask_Implementation(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	FTimerHandle MaybeClearTimer;
	if (ActiveHandles.RemoveAndCopyValue(CurrentState, MaybeClearTimer))
	{
		ActionWorld->GetTimerManager().ClearTimer(MaybeClearTimer);
	}

	USkeletalMeshComponent* SkelMesh = GetMeshFromController(ActionInstigator);
	if (SkelMesh != nullptr && SkelMesh->AnimationData.AnimToPlay == AnimationToPlay && SkelMesh->IsPlaying())
	{
		SkelMesh->Stop();
	}
}

void UActionTask_PlayAnimation::OnAnimationEnd(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld, USkeletalMeshComponent* Mesh, EAnimationMode::Type PreviousAnimationMode)
{
	if (PreviousAnimationMode == EAnimationMode::AnimationBlueprint)
	{
		if (Mesh == GetMeshFromController(ActionInstigator))
		{
			Mesh->SetAnimationMode(PreviousAnimationMode);
		}
	}
	CompleteActionTask(ActionInstigator, CurrentState, ActionTargetActor, ActionWorld);
}

