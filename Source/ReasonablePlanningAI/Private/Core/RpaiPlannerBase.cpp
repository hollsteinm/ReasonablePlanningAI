// Troll Purse. All rights reserved.


#include "Core/RpaiPlannerBase.h"
#include "Core/RpaiTypes.h"

DECLARE_CYCLE_STAT(TEXT("Plan Chosen Goal"), STAT_PlanChosenGoal, STATGROUP_Rpai);

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

bool URpaiPlannerBase::ReceivePlanChosenGoal_Implementation(const URpaiGoalBase* TargetGoal, const URpaiState* CurrentState, const TArray<URpaiActionBase*>& GivenActions, TArray<URpaiActionBase*>& OutActions) const
{
	return false; //Never able to plan
}
