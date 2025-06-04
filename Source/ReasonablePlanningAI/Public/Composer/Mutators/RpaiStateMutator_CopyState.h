// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/RpaiComposerStateMutator.h"
#include "RpaiStateMutator_CopyState.generated.h"

/**
 * Copies on state value to another. They must be the same type.
 */
UCLASS()
class REASONABLEPLANNINGAI_API URpaiStateMutator_CopyState : public URpaiComposerStateMutator
{
   GENERATED_BODY()
   
protected:
   UPROPERTY(EditAnywhere, Category = "Rpai")
   FStateKeyValueReference StatePropertyToCopy;

   virtual void ReceiveMutate_Implementation(URpaiState* StateToChange) const override;
   virtual bool CanApply_Implementation(const URpaiState* StateToChange) const override;

public:
   FORCEINLINE void SetCopiedFromStateValue(FName StateKeyName, EStatePropertyType ExpectedValueType) { StatePropertyToCopy.StateKeyName = StateKeyName; StatePropertyToCopy.ExpectedValueType = ExpectedValueType; }
};
