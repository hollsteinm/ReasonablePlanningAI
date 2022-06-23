// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/ReasonablePlanningTypes.h"
#include "ReasonablePlanningStateMutator.generated.h"

class UReasonablePlanningState;

/**
 *  Abstract base class for implementing utility functions that mutate (change) a given state.
 */
UCLASS(Abstract, BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced)
class REASONABLEPLANNINGAI_API UReasonablePlanningStateMutator : public UObject
{
	GENERATED_BODY()
    
public:
    /**
     * If configured in a valid manner and the state has valid properties, mutate the state.
     */
    UFUNCTION(BlueprintCallable, Category = "RPAI")
    void Mutate(UReasonablePlanningState* StateToChange) const;
    
protected:
    UPROPERTY(EditAnywhere, Category = "RPAI")
    FStateKeyValueReference StatePropertyToMutate;
    
    /**
     * Override this function to implement the mutation upon the state
     */
    UFUNCTION(BlueprintNativeEvent, Category = "RPAI")
    void ReceiveMutate(UReasonablePlanningState* StateToChange) const;
    virtual void ReceiveMutate_Implementation(UReasonablePlanningState* StateToChange) const;
    
    /**
     * Override this function to implement validation checks on whether the configuration is valid and the state is valid.
     */
    UFUNCTION(BlueprintNativeEvent, Category = "RPAI")
    bool CanApply(const UReasonablePlanningState* StateToChange) const;
    virtual bool CanApply_Implementation(const UReasonablePlanningState* StateToChange) const;
};
