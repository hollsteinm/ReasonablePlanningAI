// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.

#include "Core/RpaiReasonerBase.h"
#include "Core/RpaiTypes.h"

DECLARE_CYCLE_STAT(TEXT("Reason Next Goal"), STAT_ReasonNextGoal, STATGROUP_Rpai);

URpaiGoalBase* URpaiReasonerBase::ReasonNextGoal(const TArray<URpaiGoalBase*>& Goals, const URpaiState* CurrentState) const
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

   if (Goals.Num() == 1)
   {
      return Goals[0];
   }

   return ReceiveReasonNextGoal(Goals, CurrentState);
}

URpaiGoalBase* URpaiReasonerBase::ReceiveReasonNextGoal_Implementation(const TArray<URpaiGoalBase*>& Goals, const URpaiState* CurrentState) const
{
   return nullptr; //Not Implemented
}
