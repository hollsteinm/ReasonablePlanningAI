// Troll Purse. All rights reserved.

#include "ActionTask_PlaySound.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "AIController.h"
#include "Sound/SoundCue.h"

UActionTask_PlaySound::UActionTask_PlaySound()
	: SoundToPlay(nullptr)
{

}

void UActionTask_PlaySound::ReceiveStartActionTask_Implementation(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor, UWorld* ActionWorld)
{
	if (SoundToPlay != nullptr)
	{
		if (const APawn* AIPawn = ActionInstigator->GetPawn())
		{
			UGameplayStatics::SpawnSoundAttached(SoundToPlay, AIPawn->GetRootComponent());
		}
	}
}
