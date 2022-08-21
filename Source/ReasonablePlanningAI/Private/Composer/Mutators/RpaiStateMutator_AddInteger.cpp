// Troll Purse. All rights reserved.


#include "Composer/Mutators/RpaiStateMutator_AddInteger.h"
#include "Core/RpaiState.h"

void URpaiStateMutator_AddInteger::ReceiveMutate_Implementation(URpaiState* StateToMutate) const
{
    switch(StatePropertyToMutate.ExpectedValueType)
    {
        case EStatePropertyType::Float:
        {
            float Value;
            if(StateToMutate->GetFloat(StatePropertyToMutate.StateKeyName, Value))
            {
                Value += float(ValueToAdd); //TODO: Make this safe
                StateToMutate->SetFloat(StatePropertyToMutate.StateKeyName, Value);
            }
            break;
        }
        case EStatePropertyType::Int:
        {
            int32 Value;
            if(StateToMutate->GetInt(StatePropertyToMutate.StateKeyName, Value))
            {
                Value += ValueToAdd;
                StateToMutate->SetInt(StatePropertyToMutate.StateKeyName, Value);
            }
            break;
        }
        case EStatePropertyType::Vector:
        {
            FVector Value;
            if(StateToMutate->GetVector(StatePropertyToMutate.StateKeyName, Value))
            {
                Value = Value + float(ValueToAdd);
                StateToMutate->SetVector(StatePropertyToMutate.StateKeyName, Value);
            }
            break;
        }
        default:
            break;
    }
}

bool URpaiStateMutator_AddInteger::CanApply_Implementation(const URpaiState* StateToMutate) const
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
