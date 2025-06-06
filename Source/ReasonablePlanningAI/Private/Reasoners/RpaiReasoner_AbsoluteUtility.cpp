// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.


#include "Reasoners/RpaiReasoner_AbsoluteUtility.h"
#include "Core/RpaiGoalBase.h"

URpaiGoalBase* URpaiReasoner_AbsoluteUtility::ReceiveReasonNextGoal_Implementation(const TArray<URpaiGoalBase*>& Goals, const URpaiState* CurrentState) const
{
    URpaiGoalBase* SelectedGoal = nullptr;
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
