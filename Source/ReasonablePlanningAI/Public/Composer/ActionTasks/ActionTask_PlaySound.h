// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/ReasonablePlanningActionTaskBase.h"
#include "ActionTask_PlaySound.generated.h"

class USoundCue;

/**
 * Fire and forget a sound, will not wait for the sound to finish playing.
 */
UCLASS()
class REASONABLEPLANNINGAI_API UActionTask_PlaySound : public UReasonablePlanningActionTaskBase
{
	GENERATED_BODY()

public:
	UActionTask_PlaySound();

protected:
	virtual void ReceiveStartActionTask_Implementation(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr) override;

	UPROPERTY(Category = "RPAI", EditAnywhere)
	USoundCue* SoundToPlay;
	
};
