// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/ReasonablePlanningStateMutator.h"
#include "StateMutator_AddValue.generated.h"

/**
 *  Base class for mutators that add a value to the specified state. To support subtraction, simply use a negative value.
 */
UCLASS(Abstract)
class REASONABLEPLANNINGAI_API UStateMutator_AddValue : public UReasonablePlanningStateMutator
{
	GENERATED_BODY()
};
