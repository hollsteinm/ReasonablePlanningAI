// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ReasonablePlanningTypes.h"
#include "ReasonablePlanningActionTaskBase.h"
#include "GameplayTaskOwnerInterface.h"
#include "ReasonablePlanningActionTask.generated.h"

/**
 * Data driven action task with data values pulled from the state interface. Supports Gameplay Tasks
 */
UCLASS(Blueprintable, BlueprintType)
class REASONABLEPLANNINGAI_API UReasonablePlanningActionTask : public UReasonablePlanningActionTaskBase
{
	GENERATED_BODY()

public:
	UReasonablePlanningActionTask();

protected:
	UPROPERTY(EditAnywhere, Category = "ReasonablePlanningAI")
	FStateKeyValueReference ActionTaskStateKeyValueReference;
};
