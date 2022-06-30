// Troll Purse. All rights reserved.


#include "Reasoners/Reasoner_AbsoluteUtility.h"

UReasoner_AbsoluteUtility::Reasoner_AbsoluteUtility()
    : bUseWeight(true)
{
    
}

UReasonablePlanningGoalBase* UReasoner_AbsoluteUtility::ReceiveReasonNextGoal_Implementation(const TArray<UReasonablePlanningGoalBase*>& Goals, const UReasonablePlanningState* CurrentState) const
{
    UReasonablePlanningGoalBase* SelectedGoal = nullptr;
    float HighestWeight = TNumericLimits<float>::Min();
    
    if(bUseWeight)
    {
        for (const auto Goal : Goals)
        {
            if (Goal->IsApplicable(CurrentState))
            {
                float Weight = Goal->GetWeight(CurrentState);
                if(Weight > HighestWeight)
                {
                    HighestWeight = Weight;
                    SelectedGoal = Goal;
                }
            }
        }

    }
    else
    {
        for (const auto Goal : Goals)
        {
            if(Goal->IsApplicable(CurrentState))
            {
                float CategoryAsFloat = float(Goal->GetCateogry());
                if(CategoryAsFloat > HighestWeight)
                {
                    HighestWeight = CategoryAsFloat;
                    SelectedGoal = Goal;
                }
            }
        }
    }
    
    return SelectedGoal;
}
