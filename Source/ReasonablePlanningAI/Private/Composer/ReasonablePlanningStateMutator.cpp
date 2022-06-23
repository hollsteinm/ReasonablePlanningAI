// Troll Purse. All rights reserved.


#include "Composer/ReasonablePlanningStateMutator.h"
#include "Core/ReasonablePlanningState.h"
#include "Core/ReasonablePlanningTypes.h"

void UReasonablePlanningStateMutator::Mutate(UReasonablePlanningState* StateToChange) const
{
    if(StateToChange->HasReferencedState(StatePropertyToMutate) && CanApply(StateToChange))
    {
        ReceiveMutate(StateToChange);
    }
}

void UReasonablePlanningStateMutator::ReceiveMutate_Implementation(UReasonablePlanningState* StateToChange) const
{
    UE_LOG(LogRPAI, Warning, TEXT("ReceiveMutate not overriden or base class function called unnecessarily."));
}

bool UReasonablePlanningStateMutator::CanApply_Implementation(const UReasonablePlanningState* StateToChange) const
{
    return false;
}
