// Radaway Software LLC. 2025. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/RpaiGoalBase.h"
#include "RpaiComposerGoal.generated.h"

class URpaiComposerDistanceBase;
class URpaiComposerWeightBase;
class URpaiComposerStateQuery;

/**
 * A Goal is used to help determine what and AI should do. Goals are used by Reasoner classes to set the foundation for a target state
 * to pass along to a Planner class.
 */
UCLASS(BlueprintType, Blueprintable)
class REASONABLEPLANNINGAI_API URpaiComposerGoal : public URpaiGoalBase
{
	GENERATED_BODY()
	
protected:

    /** Begin ReasonablePlanningGoalBase*/
    virtual bool ReceiveIsInDesiredState_Implementation(const URpaiState* GivenState) const override;
    virtual float ReceiveGetDistanceToDesiredState_Implementation(const URpaiState* GivenState) const override;
    virtual bool ReceiveIsApplicable_Implementation(const URpaiState* GivenState) const override;
    virtual float ReceiveGetWeight_Implementation(const URpaiState* GivenState) const override;
    /** End ReasonablePlanningGoalbase*/
    
    /** Determines the amount of effort left to accomplish this goal.*/
    UPROPERTY(EditAnywhere, Category = "Rpai")
    URpaiComposerDistanceBase* DistanceCalculator;
    
    /** Determines the utility (value) of persuing this goal*/
    UPROPERTY(EditAnywhere, Category = "Rpai")
    URpaiComposerWeightBase* Weight;
    
    /** The queries to execute to determine if this goal is applicable to the given state. All queries must succeed to for the goal to be considered for the given state permutation. Used for planning as well as runtime. The empty list implies always applicable. */
    UPROPERTY(EditAnywhere, Category = "Rpai")
    URpaiComposerStateQuery* IsApplicableQuery;
    
    /** The queries to execute to determine if this goal is satisfied by the given state. All queries must succeed to for the goal to be considered satisfied given state permutation. Used for planning as well as runtime. The empty list implies always true.*/
    UPROPERTY(EditAnywhere, Category = "Rpai")
    URpaiComposerStateQuery* IsInDesiredStateQuery;
    
public:
    FORCEINLINE void SetDistanceCalculator(URpaiComposerDistanceBase* NewDistanceCalculator) { DistanceCalculator = NewDistanceCalculator; }
    
    FORCEINLINE void SetWeight(URpaiComposerWeightBase* NewWeight) { Weight = NewWeight; }
    
    FORCEINLINE void SetIsApplicableQuery(URpaiComposerStateQuery* NewIsApplicableQuery) { IsApplicableQuery = NewIsApplicableQuery; }
    
    FORCEINLINE void SetIsInDesiredStateQuery(URpaiComposerStateQuery* NewIsInDesiredStateQuery) { IsInDesiredStateQuery = NewIsInDesiredStateQuery; }
};
