// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BrainComponent.h"
#include "RPAIComposerBrainComponent.generated.h"

class UReasonablePlanningGoalBase;
class UReasonablePlanningActionBase;
class UReasonablePlanningAIBehavior;
class UReasonablePlanningState;

UCLASS(Blueprintable, BlueprintType, ClassGroup = (AI), meta = (BlueprintSpawnableComponent))
class REASONABLEPLANNINGAI_API URPAIComposerBrainComponent : public UBrainComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URPAIComposerBrainComponent();

	UFUNCTION(BlueprintCallable, Category = "RPAI")
	void SetReasonablePlanningBehavior(UReasonablePlanningAIBehavior* NewBehavior);

	// Brain Component
	virtual void StartLogic() override;
	virtual void RestartLogic() override;
	virtual void StopLogic(const FString& Reason) override;
	virtual void Cleanup() override;
	virtual void PauseLogic(const FString& Reason) override;
	virtual EAILogicResuming::Type ResumeLogic(const FString& Reason) override;

	// Actor Component
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "RPAI")
	UReasonablePlanningState* LoadOrCreateStateFromAi();

protected:
	UFUNCTION(BlueprintCallable, Category = "RPAI")
	virtual void OnActionCompleted(UReasonablePlanningActionBase* CompletedAction, AAIController* ActionInstigator, UReasonablePlanningState* CompletedOnState);

	UFUNCTION(BlueprintCallable, Category = "RPAI")
	virtual void OnActionCancelled(UReasonablePlanningActionBase* CancelledAction, AAIController* ActionInstigator, UReasonablePlanningState* CompletedOnState);

	UFUNCTION(BlueprintCallable, Category = "RPAI")
	void UnregisterOldAction(UReasonablePlanningActionBase* OldAction);

	UFUNCTION(BlueprintCallable, Category = "RPAI")
	void RegisterNewAction(UReasonablePlanningActionBase* NewAction);

	UFUNCTION(BlueprintCallable, Category = "RPAI")
	void PopNextAction();

	UFUNCTION(BlueprintNativeEvent, Category = "RPAI")
	void SetStateFromAi(UReasonablePlanningState* StateToModify) const;
	virtual void SetStateFromAi_Implementation(UReasonablePlanningState* StateToModify) const;

private:
	UPROPERTY(EditDefaultsOnly, Category = "RPAI")
	UReasonablePlanningAIBehavior* ReasonablePlanningBehavior;

	UPROPERTY(Transient)
	UReasonablePlanningActionBase* CurrentAction;

	UPROPERTY(Transient)
	TArray<UReasonablePlanningActionBase*> PlannedActions;

	UPROPERTY(Transient)
	UReasonablePlanningGoalBase* CurrentGoal;

	UPROPERTY(Transient)
	UReasonablePlanningState* CachedStateInstance;

	UPROPERTY()
	bool bIsPaused;

public:
	FORCEINLINE const UReasonablePlanningActionBase* GetCurrentAction() const { return CurrentAction; }

	FORCEINLINE const UReasonablePlanningGoalBase* GetCurrentGoal() const { return CurrentGoal; }

	FORCEINLINE bool IsPaused() const { return bIsPaused; }
};
