// Troll Purse. All rights reserved.


#include "Reasoners/Reasoner_AbsoluteUtility.h"
#include "Core/ReasonablePlanningGoalBase.h"

UReasoner_AbsoluteUtility::UReasoner_AbsoluteUtility()
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
                float CategoryAsFloat = float(Goal->GetCategory());
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
