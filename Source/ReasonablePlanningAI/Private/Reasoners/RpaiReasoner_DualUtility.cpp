// Radaway Software LLC. 2025. All rights reserved.


#include "Reasoners/RpaiReasoner_DualUtility.h"
#include "Core/RpaiState.h"
#include "Core/RpaiGoalBase.h"

struct FScoringContainer
{
   URpaiGoalBase* TheGoal;
   int32 CategoryScore;
   float UtilityScore;
   float Probability;
};

bool operator< (const FScoringContainer& LHS, const FScoringContainer& RHS)
{
   return LHS.CategoryScore < RHS.CategoryScore;
}

static TFunction<bool(const FScoringContainer&, const FScoringContainer&)> ProbabilitySort = [](const FScoringContainer& LHS, const FScoringContainer& RHS) -> bool
{
   return LHS.Probability < RHS.Probability;
};

//Algorithm implmented based on: https://www.gameaipro.com/GameAIPro2/GameAIPro2_Chapter03_Dual-Utility_Reasoning.pdf
URpaiGoalBase* URpaiReasoner_DualUtility::ReceiveReasonNextGoal_Implementation(const TArray<URpaiGoalBase*>& Goals, const URpaiState* CurrentState) const
{
   TArray<FScoringContainer> GoalDistribution;
   GoalDistribution.Reserve(Goals.Num());

   for (const auto Goal : Goals)
   {
      //Do not do weight check here so we don't invoke costly weight checks on things that may not be selected due to category
      if (Goal->IsApplicable(CurrentState))
      {
         FScoringContainer Score;
         Score.TheGoal = Goal;
         Score.CategoryScore = Goal->GetCategory();
         GoalDistribution.HeapPush(MoveTemp(Score));
      }
   }

   if (GoalDistribution.Num() <= 0)
   {
      return nullptr;
   }

   float TotalWeightInHighestCategory = 0.f;
   TArray<int32> ZeroScoresToRemove;
   const auto RemovalSort = TGreater<int32>();
   int32 Total = GoalDistribution.Num();
   auto& First = GoalDistribution[0];

   First.UtilityScore = First.TheGoal->GetWeight(CurrentState);
   if (FMath::IsNearlyZero(First.UtilityScore))
   {
      ZeroScoresToRemove.HeapPush(0, RemovalSort);
   }
   else
   {
      TotalWeightInHighestCategory += First.UtilityScore;
   }

   for (int32 Idx = 1; Idx < Total; ++Idx)
   {
      if (GoalDistribution[Idx].CategoryScore > First.CategoryScore)
      {
         GoalDistribution.RemoveAt(Idx, Total - Idx, EAllowShrinking::No);
         break;
      }

      GoalDistribution[Idx].UtilityScore = GoalDistribution[Idx].TheGoal->GetWeight(CurrentState);
      if (FMath::IsNearlyZero(GoalDistribution[Idx].UtilityScore))
      {
         ZeroScoresToRemove.HeapPush(Idx, RemovalSort);
      }
      else
      {
         TotalWeightInHighestCategory += GoalDistribution[Idx].UtilityScore;
      }
   }

   //Trim away fat and remove low scoring options within best category
   for (const auto IndexToRemove : ZeroScoresToRemove)
   {
      GoalDistribution.RemoveAt(IndexToRemove, 1, EAllowShrinking::No);
   }

   GoalDistribution.Shrink();

   if (GoalDistribution.Num() <= 0)
   {
      return nullptr;
   }

   if (GoalDistribution.Num() == 1)
   {
      return GoalDistribution[0].TheGoal;
   }

    for (auto& Action : GoalDistribution)
    {
        Action.Probability = Action.UtilityScore / TotalWeightInHighestCategory;
    }

    //TODO: Sort across a normal/gaussian distribution (bell curve)
    GoalDistribution.Sort(ProbabilitySort);

   float RandomValue = FMath::RandRange(0.f, TotalWeightInHighestCategory);
   int32 Idx = 0;
   int32 Max = GoalDistribution.Num();
   do
   {
      RandomValue -= GoalDistribution[Idx++].UtilityScore;
   } while (RandomValue > 0.f && Idx < Max);

   return GoalDistribution[Idx - 1].TheGoal;
}
