#include "AITypes.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "RpaiBTTask_DetermineNextGoal.generated.h"

/*
* Given a list of Goals and a reasoner, write to the specified blackboard key a pointer to the next goal to plan and execute.
*/
UCLASS()
class URpaiBTTask_DetermineNextGoal : public UBTTask_BlackboardBase
{
   GENERATED_BODY()

private:
   UPROPERTY(EditAnywhere, Category = "Rpai")
   class URpaiReasonerBase* Reasoner;

   UPROPERTY(EditAnywhere, Category = "Rpai")
   TArray<class URpaiGoalBase*> Goals;

public:
   URpaiBTTask_DetermineNextGoal();

   virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

   virtual void DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const override;
   virtual FString GetStaticDescription() const override;

#if WITH_EDITOR
   virtual FName GetNodeIconName() const override;
#endif // WITH_EDITOR
};