// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/RpaiComposerStateMutator.h"
#include "RpaiStateMutator_MultiplyValue.generated.h"

/**
 * Base class to apply scalar multiplication to state values. To divide, simply use a decimal value.
 */
UCLASS(Abstract)
class REASONABLEPLANNINGAI_API URpaiStateMutator_MultiplyValue : public URpaiComposerStateMutator
{
   GENERATED_BODY()
};
