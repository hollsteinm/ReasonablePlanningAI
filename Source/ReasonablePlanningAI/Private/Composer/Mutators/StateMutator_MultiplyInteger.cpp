// Troll Purse. All rights reserved.


#include "Composer/Mutators/StateMutator_MultiplyInteger.h"
#include "Core/ReasonablePlanningState.h"

void UStateMutator_MultiplyInteger::ReceiveMutate_Implementation(UReasonablePlanningState* StateToMutate) const
{
    switch(StatePropertyToMutate.ExpectedValueType)
    {
        case EStatePropertyType::Float:
        {
            float Value;
            if(StateToMutate->GetFloat(StatePropertyToMutate.StateKeyName, Value))
            {
                Value *= float(MultiplierValue);
                StateToMutate->SetFloat(StatePropertyToMutate.StateKeyName, Value);
            }
            break;
        }
        case EStatePropertyType::Int:
        {
            int32 Value;
            if(StateToMutate->GetInt(StatePropertyToMutate.StateKeyName, Value))
            {
                Value *= MultiplierValue;
                StateToMutate->SetInt(StatePropertyToMutate.StateKeyName, Value);
            }
            break;
        }
        case EStatePropertyType::Vector:
        {
            FVector Value;
            if(StateToMutate->GetVector(StatePropertyToMutate.StateKeyName, Value))
            {
                Value *= float(MultiplierValue);
                StateToMutate->SetVector(StatePropertyToMutate.StateKeyName, Value);
            }
            break;
        }
        default:
            break;
    }
}

bool UStateMutator_MultiplyInteger::CanApply_Implementation(const UReasonablePlanningState* StateToMutate) const
{
    switch(StatePropertyToMutate.ExpectedValueType)
    {
        case EStatePropertyType::Float:
            return StateToMutate->HasFloat(StatePropertyToMutate.StateKeyName);
        case EStatePropertyType::Int:
            return StateToMutate->HasInt(StatePropertyToMutate.StateKeyName);
        case EStatePropertyType::Vector:
            return StateToMutate->HasVector(StatePropertyToMutate.StateKeyName);
        default:
            return false;
    }
}
