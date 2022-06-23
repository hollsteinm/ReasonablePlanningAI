// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/ReasonablePlanningStateMutator.h"
#include "StateMutator_MultiplyValue.generated.h"

/**
 * Base class to apply scalar multiplication to state values. To divide, simply use a decimal value.
 */
UCLASS(Abstract)
class REASONABLEPLANNINGAI_API UStateMutator_MultiplyValue : public UReasonablePlanningStateMutator
{
	GENERATED_BODY()
};
