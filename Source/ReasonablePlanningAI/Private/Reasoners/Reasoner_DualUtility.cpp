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
	if (Goals.Num() <= 0)
	{
		return nullptr;
	}

	if (Goals.Num() == 1)
	{
		return Goals[0];
	}

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
	int32 Top = GoalDistribution[0].CategoryScore;
	TArray<int32> ZeroScoresToRemove;

	for (int32 Idx = 0; Idx < GoalDistribution.Num(); ++Idx)
	{
		if (GoalDistribution[Idx].CategoryScore != Top)
		{
			Top = Idx; //reuse top to indicate end of array
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
	GoalDistribution.RemoveAt(Top, GoalDistribution.Num() - Top, true);
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

	if (GoalDistribution.Num() > 2)
	{
		for (auto& Action : GoalDistribution)
		{
			Action.Probability = Action.UtilityScore / TotalWeightInHighestCategory;
		}

		//TODO: Sort across a normal/gaussian distribution (bell curve)
		GoalDistribution.Sort(ProbabilitySort);
	}

	float RandomValue = FMath::RandRange(0.f, TotalWeightInHighestCategory);
	int32 Idx = 0;
	do
	{
		RandomValue -= GoalDistribution[Idx++].UtilityScore;
	} while (!FMath::IsNearlyZero(RandomValue) && Idx < Top);

	return GoalDistribution[Idx - 1].TheGoal;
}
