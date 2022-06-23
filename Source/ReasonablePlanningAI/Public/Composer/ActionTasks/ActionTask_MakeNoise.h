// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/ReasonablePlanningActionTaskBase.h"
#include "ActionTask_MakeNoise.generated.h"

/**
 * Causes an actor controlled by the AI controller to play a noise.
 */
UCLASS()
class REASONABLEPLANNINGAI_API UActionTask_MakeNoise : public UReasonablePlanningActionTaskBase
{
	GENERATED_BODY()

public:
	UActionTask_MakeNoise();

protected:
	virtual void ReceiveStartActionTask_Implementation(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr) override;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0", ClampMax = "1", UIMin = "0", UIMax = "1"), Category = "RPAI")
	float Loudness;
};
