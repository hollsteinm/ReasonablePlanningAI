// Troll Purse. All rights reserved.


#include "Composer/RpaiComposerStateMutator.h"
#include "Core/RpaiState.h"
#include "Core/RpaiTypes.h"

void URpaiComposerStateMutator::Mutate(URpaiState* StateToChange) const
{
    if(StateToChange->HasReferencedState(StatePropertyToMutate) && CanApply(StateToChange))
    {
        ReceiveMutate(StateToChange);
    }
}

void URpaiComposerStateMutator::ReceiveMutate_Implementation(URpaiState* StateToChange) const
{
    UE_LOG(LogRpai, Warning, TEXT("ReceiveMutate not overriden or base class function called unnecessarily."));
}

bool URpaiComposerStateMutator::CanApply_Implementation(const URpaiState* StateToChange) const
{
    return false;
}
