// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
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

	UPROPERTY(EditAnywhere,Instanced,Category = "Rpai")
	TArray<URpaiGoalBase*> Goals;

	UPROPERTY(EditAnywhere,Instanced,Category = "Rpai")
	TArray<URpaiActionBase*> Actions;

public:

	FORCEINLINE const URpaiReasonerBase* GetReasoner() const { return Reasoner; }

	FORCEINLINE const URpaiPlannerBase* GetPlanner() const { return Planner; }

	FORCEINLINE TSubclassOf<URpaiState> GetConstructedStateType() const { return ConstructedStateType; }

	FORCEINLINE TArray<URpaiGoalBase*> GetGoals() const { return Goals; }

	FORCEINLINE TArray<URpaiActionBase*> GetActions() { return Actions; }
};
