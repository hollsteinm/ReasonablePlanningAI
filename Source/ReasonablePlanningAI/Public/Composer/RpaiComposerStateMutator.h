// Radaway Software LLC. 2025. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/RpaiTypes.h"
#include "RpaiComposerStateMutator.generated.h"

class URpaiState;

/**
 *  Abstract base class for implementing utility functions that mutate (change) a given state.
 */
UCLASS(Abstract, BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced)
class REASONABLEPLANNINGAI_API URpaiComposerStateMutator : public UObject
{
   GENERATED_BODY()
    
public:
    /**
     * If configured in a valid manner and the state has valid properties, mutate the state.
     */
    UFUNCTION(BlueprintCallable, Category = "Rpai")
    void Mutate(URpaiState* StateToChange) const;
    
protected:
    UPROPERTY(EditAnywhere, Category = "Rpai")
    FStateKeyValueReference StatePropertyToMutate;
    
    /**
     * Override this function to implement the mutation upon the state
     */
    UFUNCTION(BlueprintNativeEvent, Category = "Rpai")
    void ReceiveMutate(URpaiState* StateToChange) const;
    virtual void ReceiveMutate_Implementation(URpaiState* StateToChange) const;
    
    /**
     * Override this function to implement validation checks on whether the configuration is valid and the state is valid.
     */
    UFUNCTION(BlueprintNativeEvent, Category = "Rpai")
    bool CanApply(const URpaiState* StateToChange) const;
    virtual bool CanApply_Implementation(const URpaiState* StateToChange) const;

public:
    FORCEINLINE void SetMutatedStateValue(FName StateKeyName, EStatePropertyType ExpectedValueType) { StatePropertyToMutate.StateKeyName = StateKeyName; StatePropertyToMutate.ExpectedValueType = ExpectedValueType; }
};
