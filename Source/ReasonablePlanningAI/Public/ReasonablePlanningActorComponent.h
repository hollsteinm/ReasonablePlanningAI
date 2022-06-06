// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BrainComponent.h"
#include "ReasonablePlanningActorComponent.generated.h"

class UReasonablePlanningGoalBase;
class UReasonablePlanningActionBase;
class UReasonablePlanningAIBehavior;
class UReasonablePlanningState;

UCLASS(ClassGroup = (AI), meta = (BlueprintSpawnableComponent))
class REASONABLEPLANNINGAI_API UReasonablePlanningActorComponent : public UBrainComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UReasonablePlanningActorComponent();

	UFUNCTION(BlueprintCallable, Category = "ReasonablePlanningAI")
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

protected:
	UFUNCTION(BlueprintCallable, Category = "ReasonablePlanningAI")
	virtual void OnActionCompleted(UReasonablePlanningActionBase* CompletedAction, AAIController* ActionInstigator, UReasonablePlanningState* CompletedOnState);

	UFUNCTION(BlueprintCallable, Category = "ReasonablePlanningAI")
	virtual void OnActionCancelled(UReasonablePlanningActionBase* CancelledAction, AAIController* ActionInstigator, UReasonablePlanningState* CompletedOnState);

	UFUNCTION(BlueprintCallable, Category = "ReasonablePlanningAI")
	void UnregisterOldAction(UReasonablePlanningActionBase* OldAction);

	UFUNCTION(BlueprintCallable, Category = "ReasonablePlanningAI")
	void RegisterNewAction(UReasonablePlanningActionBase* NewAction);

	UFUNCTION(BlueprintCallable, Category = "ReasonablePlanningAI")
	void PopNextAction();

private:
	UPROPERTY(EditDefaultsOnly, Category = "ReasonablePlanningAI")
	UReasonablePlanningAIBehavior* ReasonablePlanningBehavior;

	UPROPERTY(Transient)
	UReasonablePlanningActionBase* CurrentAction;

	UPROPERTY(Transient)
	TArray<UReasonablePlanningActionBase*> PlannedActions;

	UPROPERTY(Transient)
	UReasonablePlanningGoalBase* CurrentGoal;

	UPROPERTY(Transient)
	UReasonablePlanningState* CurrentState;

	UPROPERTY()
	bool bIsPaused;

public:
	FORCEINLINE const UReasonablePlanningActionBase* GetCurrentAction() const { return CurrentAction; }

	FORCEINLINE const UReasonablePlanningGoalBase* GetCurrentGoal() const { return CurrentGoal; }

	FORCEINLINE bool IsPaused() const { return bIsPaused; }
};
