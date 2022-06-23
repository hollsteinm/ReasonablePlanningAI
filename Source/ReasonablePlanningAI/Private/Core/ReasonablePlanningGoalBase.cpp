// Troll Purse. All rights reserved.

#include "Core/ReasonablePlanningGoalBase.h"

UReasonablePlanningGoalBase::UReasonablePlanningGoalBase()
	: Category(0)
{

}

bool UReasonablePlanningGoalBase::IsInDesiredState(const UReasonablePlanningState* GivenState) const
{
	return ReceiveIsInDesiredState(GivenState);
}

float UReasonablePlanningGoalBase::GetDistanceToDesiredState(const UReasonablePlanningState* GivenState) const
{
	return ReceiveGetDistanceToDesiredState(GivenState);
}

bool UReasonablePlanningGoalBase::IsApplicable(const UReasonablePlanningState* GivenState) const
{
	return IsPendingKill() ? false : ReceiveIsApplicable(GivenState);
}

float UReasonablePlanningGoalBase::GetWeight(const UReasonablePlanningState* GivenState) const
{
	return ReceiveGetWeight(GivenState);
}

bool UReasonablePlanningGoalBase::ReceiveIsInDesiredState_Implementation(const UReasonablePlanningState* GivenState) const
{
	return false;
}

float UReasonablePlanningGoalBase::ReceiveGetDistanceToDesiredState_Implementation(const UReasonablePlanningState* GivenState) const
{
	return TNumericLimits<float>::Max();
}

bool UReasonablePlanningGoalBase::ReceiveIsApplicable_Implementation(const UReasonablePlanningState* GivenState) const
{
	return false;
}

float UReasonablePlanningGoalBase::ReceiveGetWeight_Implementation(const UReasonablePlanningState* GivenState) const
{
	return TNumericLimits<float>::Min();
}

FString UReasonablePlanningGoalBase::GetGoalName() const
{
    return GoalName;
}
