// Radaway Software LLC. 2025. All rights reserved.

#include "Core/RpaiGoalBase.h"

URpaiGoalBase::URpaiGoalBase()
   : Category(0)
{

}

bool URpaiGoalBase::IsInDesiredState(const URpaiState* GivenState) const
{
   return ReceiveIsInDesiredState(GivenState);
}

float URpaiGoalBase::GetDistanceToDesiredState(const URpaiState* GivenState) const
{
   return ReceiveGetDistanceToDesiredState(GivenState);
}

bool URpaiGoalBase::IsApplicable(const URpaiState* GivenState) const
{
   return IsValid(this) && ReceiveIsApplicable(GivenState);
}

float URpaiGoalBase::GetWeight(const URpaiState* GivenState) const
{
   return ReceiveGetWeight(GivenState);
}

bool URpaiGoalBase::ReceiveIsInDesiredState_Implementation(const URpaiState* GivenState) const
{
   return false;
}

float URpaiGoalBase::ReceiveGetDistanceToDesiredState_Implementation(const URpaiState* GivenState) const
{
   return 0.f;
}

bool URpaiGoalBase::ReceiveIsApplicable_Implementation(const URpaiState* GivenState) const
{
   return false;
}

float URpaiGoalBase::ReceiveGetWeight_Implementation(const URpaiState* GivenState) const
{
   return 0.f;
}

FString URpaiGoalBase::GetGoalName() const
{
    return GoalName;
}
