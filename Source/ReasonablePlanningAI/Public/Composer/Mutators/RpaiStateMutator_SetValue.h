// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/RpaiComposerStateMutator.h"
#include "RpaiStateMutator_SetValue.generated.h"

/**
 * Base class for setting a value on the state. Implement this to introduce setting values of new types.
 */
UCLASS(Abstract)
class REASONABLEPLANNINGAI_API URpaiStateMutator_SetValue : public URpaiComposerStateMutator
{
	GENERATED_BODY()
};
