// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BrainComponent.h"
#include "RpaiTypes.h"
#include "RpaiBrainComponent.generated.h"

/**
 * Base class for running the Reasonable Planning AI logic on an given AI agent (Character/Controller).
 */
UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = (AI))
class REASONABLEPLANNINGAI_API URpaiBrainComponent : public UBrainComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URpaiBrainComponent();

	// Brain Component
	virtual void StartLogic() override;
	virtual void RestartLogic() override;
	virtual void StopLogic(const FString& Reason) override;
	virtual void Cleanup() override;
	virtual void PauseLogic(const FString& Reason) override;
	virtual EAILogicResuming::Type ResumeLogic(const FString& Reason) override;
	virtual FString GetDebugInfoString() const override;

	// Actor Component
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type Reason) override;

	UFUNCTION(BlueprintCallable, Category = "Rpai")
	URpaiState* LoadOrCreateStateFromAi();

protected:
	UFUNCTION(BlueprintCallable, Category = "Rpai")
	virtual void OnActionCompleted(URpaiActionBase* CompletedAction, AAIController* ActionInstigator, URpaiState* CompletedOnState);

	UFUNCTION(BlueprintCallable, Category = "Rpai")
	virtual void OnActionCancelled(URpaiActionBase* CancelledAction, AAIController* ActionInstigator, URpaiState* CompletedOnState,bool bCancelShouldExitPlan);

	UFUNCTION(BlueprintCallable, Category = "Rpai")
	void UnregisterOldAction(URpaiActionBase* OldAction);

	UFUNCTION(BlueprintCallable, Category = "Rpai")
	void RegisterNewAction(URpaiActionBase* NewAction);

	UFUNCTION(BlueprintCallable, Category = "Rpai")
	void PopNextAction();

	UFUNCTION(BlueprintNativeEvent, Category = "Rpai")
	void SetStateFromAi(URpaiState* StateToModify) const;
	virtual void SetStateFromAi_Implementation(URpaiState* StateToModify) const;

	// Methods for getting execution implementations. By using functions instead of fields we allow for different approaches of getting the primary drivers, be it
	// from a data asset, a subsystem, a cache, or instanced data.

	UFUNCTION(BlueprintNativeEvent, Category = "Rpai")
	const URpaiReasonerBase* AcquireReasoner();
	virtual const URpaiReasonerBase* AcquireReasoner_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Rpai")
	const URpaiPlannerBase* AcquirePlanner();
	virtual const URpaiPlannerBase* AcquirePlanner_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Rpai")
	void AcquireGoals(TArray<URpaiGoalBase*>& OutGoals);
	virtual void AcquireGoals_Implementation(TArray<URpaiGoalBase*>& OutGoals);

	UFUNCTION(BlueprintNativeEvent, Category = "Rpai")
	void AcquireActions(TArray<URpaiActionBase*>& OutActions);
	virtual void AcquireActions_Implementation(TArray<URpaiActionBase*>& OutActions);

	UFUNCTION(BlueprintNativeEvent, Category = "Rpai")
	TSubclassOf<URpaiState> GetStateType();
	virtual TSubclassOf<URpaiState> GetStateType_Implementation();

private:
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

	FTimerHandle PlanningDebounce;

	FRpaiMemory ComponentActionMemory;

	FRpaiMemoryStruct CurrentActionMemory;
	
public:
	FORCEINLINE const URpaiActionBase* GetCurrentAction() const { return CurrentAction; }

	FORCEINLINE const URpaiGoalBase* GetCurrentGoal() const { return CurrentGoal; }

	FORCEINLINE bool IsPaused() const { return bIsPaused; }
};
