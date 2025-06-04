// Radaway Software LLC. 2025. All rights reserved.


#include "Core/RpaiPlannerBase.h"
#include "Core/RpaiTypes.h"

DECLARE_CYCLE_STAT(TEXT("Plan Chosen Goal"), STAT_PlanChosenGoal, STATGROUP_Rpai);
DECLARE_CYCLE_STAT(TEXT("Start Plan Chosen Goal"), STAT_StartPlanChosenGoal, STATGROUP_Rpai);
DECLARE_CYCLE_STAT(TEXT("Tick Plan Chosen Goal"), STAT_TickPlanChosenGoal, STATGROUP_Rpai);

URpaiPlannerBase::URpaiPlannerBase()
   : PlannerMemoryStructType(nullptr)
{

}

bool URpaiPlannerBase::PlanChosenGoal(const URpaiGoalBase* TargetGoal, const URpaiState* CurrentState, const TArray<URpaiActionBase*>& GivenActions, TArray<URpaiActionBase*>& OutActions) const
{
   SCOPE_CYCLE_COUNTER(STAT_PlanChosenGoal);
   if (TargetGoal == nullptr)
   {
      return false;
   }

   if (CurrentState == nullptr)
   {
      return false;
   }

   if (GivenActions.Num() <= 0)
   {
      return false;
   }

   OutActions.Empty();
   return ReceivePlanChosenGoal(TargetGoal, CurrentState, GivenActions, OutActions);
}

FRpaiMemoryStruct URpaiPlannerBase::AllocateMemorySlice(FRpaiMemory& FromMemory) const
{
   if (PlannerMemoryStructType == nullptr)
   {
      return FRpaiMemoryStruct();
   }
   return FRpaiMemoryStruct(&FromMemory, PlannerMemoryStructType);
}

ERpaiPlannerResult URpaiPlannerBase::StartGoalPlanning(const URpaiGoalBase* TargetGoal, const URpaiState* CurrentState, const TArray<URpaiActionBase*>& GivenActions, TArray<URpaiActionBase*>& OutActions, FRpaiMemoryStruct PlannerMemory) const
{
   SCOPE_CYCLE_COUNTER(STAT_StartPlanChosenGoal);
   check(PlannerMemoryStructType == nullptr ? true : PlannerMemory.IsValid() && PlannerMemory.IsCompatibleType(PlannerMemoryStructType));
   OutActions.Empty();
   return ReceiveStartGoalPlanning(TargetGoal, CurrentState, GivenActions, OutActions, PlannerMemory);
}

ERpaiPlannerResult URpaiPlannerBase::TickGoalPlanning(const URpaiGoalBase* TargetGoal, const URpaiState* CurrentState, const TArray<URpaiActionBase*>& GivenActions, TArray<URpaiActionBase*>& OutActions, FRpaiMemoryStruct PlannerMemory) const
{
   SCOPE_CYCLE_COUNTER(STAT_TickPlanChosenGoal);
   check(PlannerMemoryStructType == nullptr ? true : PlannerMemory.IsValid() && PlannerMemory.IsCompatibleType(PlannerMemoryStructType));
   return ReceiveTickGoalPlanning(TargetGoal, CurrentState, GivenActions, OutActions, PlannerMemory);
}

ERpaiPlannerResult URpaiPlannerBase::CancelGoalPlanning(const URpaiGoalBase* TargetGoal, const URpaiState* CurrentState, const TArray<URpaiActionBase*>& GivenActions, TArray<URpaiActionBase*>& OutActions, FRpaiMemoryStruct PlannerMemory) const
{
   check(PlannerMemoryStructType == nullptr ? true : PlannerMemory.IsValid() && PlannerMemory.IsCompatibleType(PlannerMemoryStructType));
   OutActions.Empty();
   return ReceiveCancelGoalPlanning(TargetGoal, CurrentState, GivenActions, OutActions, PlannerMemory);
}

bool URpaiPlannerBase::ReceivePlanChosenGoal_Implementation(const URpaiGoalBase* TargetGoal, const URpaiState* CurrentState, const TArray<URpaiActionBase*>& GivenActions, TArray<URpaiActionBase*>& OutActions) const
{
   return false; //Never able to plan
}

ERpaiPlannerResult URpaiPlannerBase::ReceiveStartGoalPlanning_Implementation(const URpaiGoalBase* TargetGoal, const URpaiState* CurrentState, const TArray<URpaiActionBase*>& GivenActions, TArray<URpaiActionBase*>& OutActions, FRpaiMemoryStruct PlannerMemory) const
{
   return ReceivePlanChosenGoal(TargetGoal, CurrentState, GivenActions, OutActions) ? ERpaiPlannerResult::CompletedSuccess : ERpaiPlannerResult::CompletedFailure;
}

ERpaiPlannerResult URpaiPlannerBase::ReceiveTickGoalPlanning_Implementation(const URpaiGoalBase* TargetGoal, const URpaiState* CurrentState, const TArray<URpaiActionBase*>& GivenActions, TArray<URpaiActionBase*>& OutActions, FRpaiMemoryStruct PlannerMemory) const
{
   return ERpaiPlannerResult::Invalid;
}

ERpaiPlannerResult URpaiPlannerBase::ReceiveCancelGoalPlanning_Implementation(const URpaiGoalBase* TargetGoal, const URpaiState* CurrentState, const TArray<URpaiActionBase*>& GivenActions, TArray<URpaiActionBase*>& OutActions, FRpaiMemoryStruct PlannerMemory) const
{
   return ERpaiPlannerResult::Invalid;
}