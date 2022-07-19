// Troll Purse. All rights reserved.


#include "Reasoners/Reasoner_DualUtility.h"
#include "Core/ReasonablePlanningState.h"
#include "Core/ReasonablePlanningGoalBase.h"

struct FScoringContainer
{
	UReasonablePlanningGoalBase* TheGoal;
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
UReasonablePlanningGoalBase* UReasoner_DualUtility::ReceiveReasonNextGoal_Implementation(const TArray<UReasonablePlanningGoalBase*>& Goals, const UReasonablePlanningState* CurrentState) const
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
	int32 Total = GoalDistribution.Num();
	auto& First = GoalDistribution[0];

	First.UtilityScore = First.TheGoal->GetWeight(CurrentState);
	if (FMath::IsNearlyZero(First.UtilityScore))
	{
		ZeroScoresToRemove.Push(0);
	}
	else
	{
		TotalWeightInHighestCategory += First.UtilityScore;
	}

	for (int32 Idx = 1; Idx < Total; ++Idx)
	{
		if (GoalDistribution[Idx].CategoryScore > First.CategoryScore)
		{
			GoalDistribution.RemoveAt(Idx, Total - Idx, true);
			break;
		}

		GoalDistribution[Idx].UtilityScore = GoalDistribution[Idx].TheGoal->GetWeight(CurrentState);
		if (FMath::IsNearlyZero(GoalDistribution[Idx].UtilityScore))
		{
			ZeroScoresToRemove.Push(Idx);
		}
		else
		{
			TotalWeightInHighestCategory += GoalDistribution[Idx].UtilityScore;
		}
	}

	//Trim away fat and remove low scoring options within best category
	for (const auto IndexToRemove : ZeroScoresToRemove)
	{
		GoalDistribution.RemoveAt(IndexToRemove);
	}

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
	} while (!FMath::IsNearlyZero(RandomValue) && Idx < Max);

	return GoalDistribution[Idx - 1].TheGoal;
}
