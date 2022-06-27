// Troll Purse. All rights reserved.


#include "Composer/Weights/Weight_Distance.h"
#include "Composer/ReasonablePlanningDistance.h"

float UWeight_Distance::ReceiveExecutionWeight_Implementation(const UReasonablePlanningState* GivenState) const
{
    check(GivenState != nullptr);
    check(Distance != nullptr);
    return Distance->CalculateDistance(GivenState);
}
