#include "AITypes.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Core/RpaiTypes.h"
#include "RpaiBTTask_ExecutePlannedGoal.generated.h"

class URpaiActionBase;
class URpaiState;

struct FExecutePlannedGoalMemory
{
	TArray<URpaiActionBase*> RemainingPlan;
	URpaiActionBase* CurrentAction;
	FRpaiMemoryStruct ActionMemory;
	FRpaiMemoryStruct PlanMemory;
	FRpaiMemory ExecutionMemory;
	ERpaiPlannerResult LastPlanningResult;
	bool bShouldFail;
	FORCEINLINE bool IsExecuting() const { return LastPlanningResult == ERpaiPlannerResult::CompletedSuccess; }
};

/*
* Given an input blackboard key of a specified Goal, create a plan and execute on plan.
*/
UCLASS()
class URpaiBTTask_ExecutePlannedGoal : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

private:
	/**
	* Unfortunately, keep track of memory to controllers to get memory in delegate events (which do not allow functors with values passed in)
	**/
	TMap<AAIController*, FExecutePlannedGoalMemory*> ControllerToMemory;

	/**
	* The planner to execute when this task is executed.
	**/
	UPROPERTY(EditAnywhere, Category = "Rpai")
		class URpaiPlannerBase* Planner;

	/*
	* The actions that are considered for the planner given the specified goal.
	*/
	UPROPERTY(EditAnywhere, Category = "Rpai")
		TArray<URpaiActionBase*> Actions;

	/*
	* When the currently running action is completed, this event will be triggered.
	*/
	UFUNCTION()
	void OnActionCompleted(URpaiActionBase* CompletedAction, AAIController* ActionInstigator, URpaiState* CompletedOnState);

	/*
	* When the currently running action is cancelled, this event will be triggered.
	*/
	UFUNCTION()
	void OnActionCancelled(URpaiActionBase* CompletedAction, AAIController* ActionInstigator, URpaiState* CompletedOnState, bool bCancelShouldExitPlan);

protected:
	/*
	* When any action completes, this will be triggered. Removes state management complexity from any children.
	*/
	virtual void OnActionEvent(URpaiActionBase* CompletedAction, AAIController* ActionInstigator, URpaiState* CompletedOnState, bool bCompleted, FExecutePlannedGoalMemory* TaskMemory, bool bShouldFail);


public:
	URpaiBTTask_ExecutePlannedGoal();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual uint16 GetInstanceMemorySize() const override;

	virtual void DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const override;
	virtual FString GetStaticDescription() const override;

#if WITH_EDITOR
	virtual FName GetNodeIconName() const override;
#endif // WITH_EDITOR
};