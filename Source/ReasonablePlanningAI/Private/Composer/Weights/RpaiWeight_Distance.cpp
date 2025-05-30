// Radaway Software LLC. 2025. All rights reserved.


#include "Composer/Weights/RpaiWeight_Distance.h"
#include "Composer/RpaiComposerDistance.h"

float URpaiWeight_Distance::ReceiveExecutionWeight_Implementation(const URpaiState* GivenState) const
{
    check(GivenState != nullptr);
    check(Distance != nullptr);
    return Distance->CalculateDistance(GivenState);
}
