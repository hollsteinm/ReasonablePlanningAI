// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/RpaiComposerStateMutator.h"
#include "RpaiStateMutator_AddValue.generated.h"

/**
 *  Base class for mutators that add a value to the specified state. To support subtraction, simply use a negative value.
 */
UCLASS(Abstract)
class REASONABLEPLANNINGAI_API URpaiStateMutator_AddValue : public URpaiComposerStateMutator
{
	GENERATED_BODY()
};
