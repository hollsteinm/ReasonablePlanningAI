// Troll Purse. All rights reserved.

#include "ReasonablePlanningReasonerBase.h"
#include "ReasonablePlanningTypes.h"

DECLARE_CYCLE_STAT(TEXT("Reason Next Goal"), STAT_ReasonNextGoal, STATGROUP_RPAI);

UReasonablePlanningGoalBase* UReasonablePlanningReasonerBase::ReasonNextGoal(const TArray<UReasonablePlanningGoalBase*>& Goals, const UReasonablePlanningState* CurrentState) const
{
	SCOPE_CYCLE_COUNTER(STAT_ReasonNextGoal);
	if (Goals.Num() <= 0)
	{
		return nullptr;
	}

	if (CurrentState == nullptr)
	{
		return nullptr;
	}

	return ReceiveReasonNextGoal(Goals, CurrentState);
}

UReasonablePlanningGoalBase* UReasonablePlanningReasonerBase::ReceiveReasonNextGoal_Implementation(const TArray<UReasonablePlanningGoalBase*>& Goals, const UReasonablePlanningState* CurrentState) const
{
	return nullptr; //Not Implemented
}