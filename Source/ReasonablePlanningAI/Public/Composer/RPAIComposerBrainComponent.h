// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/RpaiBrainComponent.h"
#include "RpaiComposerBrainComponent.generated.h"

class URpaiGoalBase;
class URpaiActionBase;
class URpaiComposerBehavior;
class URpaiState;
class URpaiReasonerBase;
class URpaiPlannerBase;

UCLASS(Blueprintable, BlueprintType, ClassGroup = (AI), meta = (BlueprintSpawnableComponent))
class REASONABLEPLANNINGAI_API URpaiComposerBrainComponent : public URpaiBrainComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URpaiComposerBrainComponent();

	UFUNCTION(BlueprintCallable, Category = "Rpai")
	void SetReasonablePlanningBehavior(URpaiComposerBehavior* NewBehavior);

protected:
	virtual const URpaiReasonerBase* AcquireReasoner_Implementation();
	virtual const URpaiPlannerBase* AcquirePlanner_Implementation();
	virtual void AcquireGoals_Implementation(TArray<URpaiGoalBase*>& OutGoals);
	virtual void AcquireActions_Implementation(TArray<URpaiActionBase*>& OutActions);
	virtual TSubclassOf<URpaiState> GetStateType_Implementation();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Rpai")
	URpaiComposerBehavior* ReasonablePlanningBehavior;
};
