// Troll Purse. All rights reserved.


#include "Reasoners/Reasoner_AbsoluteUtility.h"
#include "Core/ReasonablePlanningGoalBase.h"

UReasonablePlanningGoalBase* UReasoner_AbsoluteUtility::ReceiveReasonNextGoal_Implementation(const TArray<UReasonablePlanningGoalBase*>& Goals, const UReasonablePlanningState* CurrentState) const
{
    UReasonablePlanningGoalBase* SelectedGoal = nullptr;
    float HighestWeight = TNumericLimits<float>::Min();
    
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
    
    return SelectedGoal;
}
