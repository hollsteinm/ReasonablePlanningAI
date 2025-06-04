// Radaway Software LLC. 2025. All rights reserved.


#include "Composer/Weights/RpaiWeight_Float.h"
#include "Core/RpaiState.h"

float URpaiWeight_Float::ReceiveExecutionWeight_Implementation(const URpaiState* GivenState) const
{
   float ReturnValue = 0.f;
   if (!GivenState->GetFloat(WeightStateKeyValueReference.StateKeyName, ReturnValue))
   {
      return 0.f;
   }
   return ReturnValue;
}
