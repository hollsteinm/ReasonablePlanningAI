// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/ReasonablePlanningGoalBase.h"
#include "ReasonablePlanningGoal.generated.h"

class UReasonablePlanningDistanceBase;
class UReasonablePlanningWeightBase;
class UReasonablePlanningStateQuery;

/**
 * A Goal is used to help determine what and AI should do. Goals are used by Reasoner classes to set the foundation for a target state
 * to pass along to a Planner class.
 */
UCLASS(BlueprintType, Blueprintable)
class REASONABLEPLANNINGAI_API UReasonablePlanningGoal : public UReasonablePlanningGoalBase
{
	GENERATED_BODY()
	
protected:

    /** Begin ReasonablePlanningGoalBase*/
    virtual bool ReceiveIsInDesiredState_Implementation(const UReasonablePlanningState* GivenState) const override;
    virtual float ReceiveGetDistanceToDesiredState_Implementation(const UReasonablePlanningState* GivenState) const override;
    virtual bool ReceiveIsApplicable_Implementation(const UReasonablePlanningState* GivenState) const override;
    virtual float ReceiveGetWeight_Implementation(const UReasonablePlanningState* GivenState) const override;
    /** End ReasonablePlanningGoalbase*/
    
    /** Determines the amount of effort left to accomplish this goal.*/
    UPROPERTY(EditAnywhere, Category = "RPAI")
    UReasonablePlanningDistanceBase* DistanceCalculator;
    
    /** Determines the utility (value) of persuing this goal*/
    UPROPERTY(EditAnywhere, Category = "RPAI")
    UReasonablePlanningWeightBase* Weight;
    
    /** The queries to execute to determine if this goal is applicable to the given state. All queries must succeed to for the goal to be considered for the given state permutation. Used for planning as well as runtime. The empty list implies always applicable. */
    UPROPERTY(EditAnywhere, Category = "RPAI")
    UReasonablePlanningStateQuery* IsApplicableQuery;
    
    /** The queries to execute to determine if this goal is satisfied by the given state. All queries must succeed to for the goal to be considered satisfied given state permutation. Used for planning as well as runtime. The empty list implies always true.*/
    UPROPERTY(EditAnywhere, Category = "RPAI")
    UReasonablePlanningStateQuery* IsInDesiredStateQuery;
    
public:
    FORCEINLINE void SetDistanceCalculator(UReasonablePlanningDistanceBase* NewDistanceCalculator) { DistanceCalculator = NewDistanceCalculator; }
    
    FORCEINLINE void SetWeight(UReasonablePlanningWeightBase* NewWeight) { Weight = NewWeight; }
    
    FORCEINLINE void SetIsApplicableQuery(UReasonablePlanningStateQuery* NewIsApplicableQuery) { IsApplicableQuery = NewIsApplicableQuery; }
    
    FORCEINLINE void SetIsInDesiredStateQuery(UReasonablePlanningStateQuery* NewIsInDesiredStateQuery) { IsInDesiredStateQuery = NewIsInDesiredStateQuery; }
};
