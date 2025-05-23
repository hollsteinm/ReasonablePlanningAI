// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BrainComponent.h"
#include "RpaiTypes.h"
#include "RpaiBrainComponent.generated.h"

#if WITH_EDITORONLY_DATA
USTRUCT(NoExport)
struct REASONABLEPLANNINGAI_API FRpaiBindingConfiguration
{
    UPROPERTY(EditAnywhere, Category=Rpai)
    TSubclassOf<AActor> SourceType;
    
    UPROPERTY(EditAnywhere, Category=Rpai)
    TArray<FString> SourceBindingPath;
    
    UPROPERTY(EditAnywhere, Category=Rpai)
    TArray<FString> TargetBindingPath;
};
#endif

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
    
    // UObject Callbacks
#if WITH_EDITOR
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	UFUNCTION(BlueprintCallable, Category = "Rpai")
	URpaiState* LoadOrCreateStateFromAi();

	UFUNCTION(BlueprintPure, Category = "Rpai")
	URpaiState* GetLastCachedState() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rpai")
	bool bUseMultiTickPlanning;

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

	UFUNCTION(BlueprintCallable, Category = "Rpai")
	void SetStateFromAi(URpaiState* StateToModify) const;

	UFUNCTION(BlueprintCallable, Category = "Rpai")
	virtual void OnComponentSetStateFromAi(URpaiState* StateToModify) const;

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
	TSubclassOf<URpaiState> GetStateType() const;
    virtual TSubclassOf<URpaiState> GetStateType_Implementation() const;

	UFUNCTION(BlueprintCallable, Category = "Rpai")
	void StartDebounceTimer();

	/** The stored state type to use for reasoning and planning. **/
	UPROPERTY(EditAnywhere, Category = Rpai)
	TSubclassOf<URpaiState> DefaultStateType;

	UFUNCTION(BlueprintCallable, Category = "Rpai")
	void ClearCachedStateInstance();

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

	FRpaiMemoryStruct CurrentPlannerMemory;

	ERpaiPlannerResult LastPlannerResultForMultiTick;
    
    const URpaiPlannerBase* DoAcquirePlanner();

	const URpaiPlannerBase* CurrentPlanner;
    
#if WITH_EDITORONLY_DATA
    UPROPERTY(EditAnywhere, Category=Rpai)
    TArray<FRpaiBindingConfiguration> StateBindingConfigurations;
#endif
    
    UPROPERTY()
    FRpaiStateBinding StateCopyBindings;
	
public:
	FORCEINLINE const URpaiActionBase* GetCurrentAction() const { return CurrentAction; }

	FORCEINLINE const URpaiGoalBase* GetCurrentGoal() const { return CurrentGoal; }

	FORCEINLINE bool IsPaused() const { return bIsPaused; }
    
    FORCEINLINE void SetCurrentStateBindings(const FRpaiStateBinding& NewBindings) { StateCopyBindings = NewBindings; }
};
