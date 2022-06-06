// Troll Purse. All rights reserved.


#include "ReasonablePlanningPlannerBase.h"
#include "ReasonablePlanningTypes.h"

DECLARE_CYCLE_STAT(TEXT("Plan Chosen Goal"), STAT_PlanChosenGoal, STATGROUP_RPAI);

bool UReasonablePlanningPlannerBase::PlanChosenGoal(const UReasonablePlanningGoalBase* TargetGoal, const UReasonablePlanningState* CurrentState, const TArray<UReasonablePlanningActionBase*>& GivenActions, TArray<UReasonablePlanningActionBase*>& OutActions) const
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

bool UReasonablePlanningPlannerBase::ReceivePlanChosenGoal_Implementation(const UReasonablePlanningGoalBase* TargetGoal, const UReasonablePlanningState* CurrentState, const TArray<UReasonablePlanningActionBase*>& GivenActions, TArray<UReasonablePlanningActionBase*>& OutActions) const
{
	return false; //Never able to plan
}