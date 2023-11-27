// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Core/RpaiState.h"
#include "RpaiComposerBehavior.generated.h"

class URpaiActionBase;
class URpaiGoalBase;
class URpaiReasonerBase;
class URpaiPlannerBase;

/**
 * Data Asset to enable configuration of a defined AI Behavior. An AI Behavior within
 * the Reasonable Planning AI module is one that defines a set of goals and a set of
 * actions that can be used as a template for many AI Pawns.
 */
UCLASS(BlueprintType)
class REASONABLEPLANNINGAI_API URpaiComposerBehavior : public UDataAsset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Rpai")
	TSubclassOf<URpaiState> ConstructedStateType;

	UPROPERTY(EditAnywhere, Category = "Rpai")
	URpaiReasonerBase* Reasoner;

	UPROPERTY(EditAnywhere, Category = "Rpai")
	URpaiPlannerBase* Planner;

	UPROPERTY(EditAnywhere,Instanced,Category = "Rpai", meta=(TitleProperty = "GoalName"))
	TArray<URpaiGoalBase*> Goals;

	UPROPERTY(EditAnywhere,Instanced,Category = "Rpai", meta=(TitleProperty = "ActionName"))
	TArray<URpaiActionBase*> Actions;

public:

	FORCEINLINE const URpaiReasonerBase* GetReasoner() const { return Reasoner; }

	FORCEINLINE const URpaiPlannerBase* GetPlanner() const { return Planner; }

	FORCEINLINE TSubclassOf<URpaiState> GetConstructedStateType() const { return ConstructedStateType; }

	FORCEINLINE TArray<URpaiGoalBase*> GetGoals() const { return Goals; }

	FORCEINLINE TArray<URpaiActionBase*> GetActions() const { return Actions; }

	FORCEINLINE void SetReasoner(URpaiReasonerBase* NewReasoner) { Reasoner = NewReasoner; }

	FORCEINLINE void SetPlanner(URpaiPlannerBase* NewPlanner) { Planner = NewPlanner; }

	FORCEINLINE void SetConstructedStateType(TSubclassOf<URpaiState> NewStateType) { ConstructedStateType = NewStateType; }

	FORCEINLINE void SetGoals(const TArray<URpaiGoalBase*>& NewGoals) { Goals = NewGoals; }

	FORCEINLINE void SetActions(TArray<URpaiActionBase*> NewActions) { Actions = NewActions; }
};
