// Troll Purse. All rights reserved.

#include "Composer/ActionTasks/RpaiActionTask_RotateTo.h"
#include "AIController.h"
#include "GameFramework/Pawn.h"
#include "Core/RpaiState.h"

URpaiActionTask_RotateTo::URpaiActionTask_RotateTo()
	: Precision(10.f)
{
	bCompleteAfterStart = false;
}

static float CalculateAngleDifferenceDot(const FVector& VectorA, const FVector& VectorB)
{
	return (VectorA.IsNearlyZero() || VectorB.IsNearlyZero())
		? 1.f
		: VectorA.CosineAngle2D(VectorB);
}

void URpaiActionTask_RotateTo::ReceiveStartActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	APawn* Pawn = ActionInstigator->GetPawn();
	if (Pawn == nullptr)
	{
		CancelActionTask(ActionInstigator, CurrentState, ActionTargetActor, ActionWorld);
		return;
	}

	FVector MaybeVector;
	FRotator MaybeRotator;
	UObject* MaybeActor = nullptr;
	const FVector PawnLocation = Pawn->GetActorLocation();

	if (CurrentState->GetVector(ActionTaskStateKeyValueReference.StateKeyName, MaybeVector))
	{
		if (FAISystem::IsValidLocation(MaybeVector))
		{
			const float AngleDifference = CalculateAngleDifferenceDot(Pawn->GetActorForwardVector()
				, (MaybeVector - PawnLocation));

			if (AngleDifference >= PrecisionDot)
			{
				CompleteActionTask(ActionInstigator, CurrentState, ActionTargetActor, ActionWorld);
				return;
			}
			else
			{
				ActionInstigator->SetFocalPoint(MaybeVector, EAIFocusPriority::Gameplay);
				return;
			}
		}
	}
	else if (CurrentState->GetObject(ActionTaskStateKeyValueReference.StateKeyName, MaybeActor))
	{
		if (MaybeActor != nullptr && MaybeActor->IsA<AActor>())
		{
			AActor* Actor = Cast<AActor>(MaybeActor);
			const float AngleDifference = CalculateAngleDifferenceDot(Pawn->GetActorForwardVector(), (Actor->GetActorLocation() - PawnLocation));

			if (AngleDifference >= PrecisionDot)
			{
				CompleteActionTask(ActionInstigator, CurrentState, ActionTargetActor, ActionWorld);
				return;
			}
			else
			{
				ActionInstigator->SetFocus(Actor, EAIFocusPriority::Gameplay);
				return;
			}
		}
	}
	else if(CurrentState->GetRotator(ActionTaskStateKeyValueReference.StateKeyName, MaybeRotator))
	{
		if (FAISystem::IsValidRotation(MaybeRotator))
		{
			const FVector DirectionVector = MaybeRotator.Vector();
			const float AngleDifference = CalculateAngleDifferenceDot(Pawn->GetActorForwardVector(), DirectionVector);

			if (AngleDifference >= PrecisionDot)
			{
				CompleteActionTask(ActionInstigator, CurrentState, ActionTargetActor, ActionWorld);
				return;
			}
			else
			{
				const FVector FocalPoint = PawnLocation + DirectionVector * 10000.0f;
				ActionInstigator->SetFocalPoint(FocalPoint, EAIFocusPriority::Gameplay);
				return;
			}
		}
	}
	CancelActionTask(ActionInstigator, CurrentState, ActionTargetActor, ActionWorld);
}

void URpaiActionTask_RotateTo::ReceiveUpdateActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, float DeltaSeconds, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	const FVector PawnDirection = ActionInstigator->GetPawn()->GetActorForwardVector();
	const FVector FocalPoint = ActionInstigator->GetFocalPointForPriority(EAIFocusPriority::Gameplay);

	if (CalculateAngleDifferenceDot(PawnDirection, FocalPoint - ActionInstigator->GetPawn()->GetActorLocation()) >= PrecisionDot)
	{
		CompleteActionTask(ActionInstigator, CurrentState, ActionTargetActor, ActionWorld);
		ActionInstigator->ClearFocus(EAIFocusPriority::Gameplay);
	}
}

void URpaiActionTask_RotateTo::ReceiveCancelActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	ActionInstigator->ClearFocus(EAIFocusPriority::Gameplay);
}

void URpaiActionTask_RotateTo::PostInitProperties()
{
	Super::PostInitProperties();
	PrecisionDot = FMath::Cos(FMath::DegreesToRadians(Precision));
}

void URpaiActionTask_RotateTo::PostLoad()
{
	Super::PostLoad();
	PrecisionDot = FMath::Cos(FMath::DegreesToRadians(Precision));
}
