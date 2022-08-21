// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BrainComponent.h"
#include "RpaiComposerBrainComponent.generated.h"

class URpaiGoalBase;
class URpaiActionBase;
class URpaiComposerBehavior;
class URpaiState;

UCLASS(Blueprintable, BlueprintType, ClassGroup = (AI), meta = (BlueprintSpawnableComponent))
class REASONABLEPLANNINGAI_API URpaiComposerBrainComponent : public UBrainComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URpaiComposerBrainComponent();

	UFUNCTION(BlueprintCallable, Category = "Rpai")
	void SetReasonablePlanningBehavior(URpaiComposerBehavior* NewBehavior);

	// Brain Component
	virtual void StartLogic() override;
	virtual void RestartLogic() override;
	virtual void StopLogic(const FString& Reason) override;
	virtual void Cleanup() override;
	virtual void PauseLogic(const FString& Reason) override;
	virtual EAILogicResuming::Type ResumeLogic(const FString& Reason) override;

	// Actor Component
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Rpai")
	URpaiState* LoadOrCreateStateFromAi();

protected:
	UFUNCTION(BlueprintCallable, Category = "Rpai")
	virtual void OnActionCompleted(URpaiActionBase* CompletedAction, AAIController* ActionInstigator, URpaiState* CompletedOnState);

	UFUNCTION(BlueprintCallable, Category = "Rpai")
	virtual void OnActionCancelled(URpaiActionBase* CancelledAction, AAIController* ActionInstigator, URpaiState* CompletedOnState);

	UFUNCTION(BlueprintCallable, Category = "Rpai")
	void UnregisterOldAction(URpaiActionBase* OldAction);

	UFUNCTION(BlueprintCallable, Category = "Rpai")
	void RegisterNewAction(URpaiActionBase* NewAction);

	UFUNCTION(BlueprintCallable, Category = "Rpai")
	void PopNextAction();

	UFUNCTION(BlueprintNativeEvent, Category = "Rpai")
	void SetStateFromAi(URpaiState* StateToModify) const;
	virtual void SetStateFromAi_Implementation(URpaiState* StateToModify) const;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Rpai")
	URpaiComposerBehavior* ReasonablePlanningBehavior;

	UPROPERTY(Transient)
	URpaiActionBase* CurrentAction;

	UPROPERTY(Transient)
	TArray<URpaiActionBase*> PlannedActions;

	UPROPERTY(Transient)
	URpaiGoalBase* CurrentGoal;

	UPROPERTY(Transient)
	URpaiState* CachedStateInstance;

	UPROPERTY()
	bool bIsPaused;

public:
	FORCEINLINE const URpaiActionBase* GetCurrentAction() const { return CurrentAction; }

	FORCEINLINE const URpaiGoalBase* GetCurrentGoal() const { return CurrentGoal; }

	FORCEINLINE bool IsPaused() const { return bIsPaused; }
};
