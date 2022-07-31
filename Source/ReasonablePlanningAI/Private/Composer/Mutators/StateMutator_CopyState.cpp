// Troll Purse. All rights reserved.


#include "Composer/Mutators/StateMutator_CopyState.h"
#include "Core/ReasonablePlanningState.h"

void UStateMutator_CopyState::ReceiveMutate_Implementation(UReasonablePlanningState* StateToChange) const
{
	switch (StatePropertyToCopy.ExpectedValueType)
	{
	case EStatePropertyType::Bool:
	{
		bool bValue = false;
		if (StateToChange->GetValueOfType(StatePropertyToCopy.StateKeyName, bValue))
		{
			StateToChange->SetValueOfType(StatePropertyToMutate.StateKeyName, bValue);
		}
	}
	break;
	case EStatePropertyType::Class:
	{
		UClass* Value = nullptr;
		if (StateToChange->GetValueOfType(StatePropertyToCopy.StateKeyName, Value))
		{
			StateToChange->SetValueOfType(StatePropertyToMutate.StateKeyName, Value);
		}
	}
	break;
	case EStatePropertyType::Float:
	{
		float Value;
		if (StateToChange->GetValueOfType(StatePropertyToCopy.StateKeyName, Value))
		{
			StateToChange->SetValueOfType(StatePropertyToMutate.StateKeyName, Value);
		}
	}
	break;
	case EStatePropertyType::Int:
	{
		int32 Value;
		if (StateToChange->GetValueOfType(StatePropertyToCopy.StateKeyName, Value))
		{
			StateToChange->SetValueOfType(StatePropertyToMutate.StateKeyName, Value);
		}
	}
	break;
	case EStatePropertyType::Name:
	{
		FName Value;
		if (StateToChange->GetValueOfType(StatePropertyToCopy.StateKeyName, Value))
		{
			StateToChange->SetValueOfType(StatePropertyToMutate.StateKeyName, Value);
		}
	}
	break;
	case EStatePropertyType::Object:
	{
		UObject* Value;
		if (StateToChange->GetValueOfType(StatePropertyToCopy.StateKeyName, Value))
		{
			StateToChange->SetValueOfType(StatePropertyToMutate.StateKeyName, Value);
		}
	}
	break;
	case EStatePropertyType::Rotator:
	{
		FRotator Value;
		if (StateToChange->GetValueOfType(StatePropertyToCopy.StateKeyName, Value))
		{
			StateToChange->SetValueOfType(StatePropertyToMutate.StateKeyName, Value);
		}
	}
	break;
	case EStatePropertyType::String:
	{
		FString Value;
		if (StateToChange->GetValueOfType(StatePropertyToCopy.StateKeyName, Value))
		{
			StateToChange->SetValueOfType(StatePropertyToMutate.StateKeyName, Value);
		}
	}
	break;
	case EStatePropertyType::Vector:
	{
		FVector Value;
		if (StateToChange->GetValueOfType(StatePropertyToCopy.StateKeyName, Value))
		{
			StateToChange->SetValueOfType(StatePropertyToMutate.StateKeyName, Value);
		}
	}
	break;
	default:
		break;
	}
}

bool UStateMutator_CopyState::CanApply_Implementation(const UReasonablePlanningState* StateToChange) const
{
	return StateToChange->HasReferencedState(StatePropertyToCopy) && StatePropertyToCopy.ExpectedValueType == StatePropertyToMutate.ExpectedValueType;
}