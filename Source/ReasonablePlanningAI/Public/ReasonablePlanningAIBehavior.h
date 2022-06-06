// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ReasonablePlanningAIBehavior.generated.h"

class UReasonablePlanningActionBase;
class UReasonablePlanningGoalBase;
class UReasonablePlanningReasonerBase;
class UReasonablePlanningPlannerBase;

/**
 * Data Asset to enable configuration of a defined AI Behavior. An AI Behavior within
 * the Reasonable Planning AI module is one that defines a set of goals and a set of
 * actions that can be used as a template for many AI Pawns.
 */
UCLASS()
class REASONABLEPLANNINGAI_API UReasonablePlanningAIBehavior : public UDataAsset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "ReasonablePlanningAI")
	UReasonablePlanningReasonerBase* Reasoner;

	UPROPERTY(EditAnywhere, Category = "ReasonablePlanningAI")
	UReasonablePlanningPlannerBase* Planner;

	UPROPERTY(EditAnywhere,Instanced,Category="ReasonablePlanningAI")
	TArray<UReasonablePlanningGoalBase*> Goals;

	UPROPERTY(EditAnywhere,Instanced,Category="ReasonablePlanningAI")
	TArray<UReasonablePlanningActionBase*> Actions;

public:
	FORCEINLINE const UReasonablePlanningReasonerBase* GetReasoner() const { return Reasoner; }

	FORCEINLINE const UReasonablePlanningPlannerBase* GetPlanner() const { return Planner; }

	FORCEINLINE TArray<UReasonablePlanningGoalBase*> GetGoals() const { return Goals; }

	FORCEINLINE TArray<UReasonablePlanningActionBase*> GetActions() { return Actions; }
};
