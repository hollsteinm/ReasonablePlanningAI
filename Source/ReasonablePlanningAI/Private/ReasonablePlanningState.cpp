// Troll Purse. All rights reserved.


#include "ReasonablePlanningState.h"

bool UReasonablePlanningState::HasReferencedState(const FStateKeyValueReference& StatePropertyReference) const
{
	return !StatePropertyReference.StateKeyName.IsNone() &&
		HasValueWithName(StatePropertyReference.StateKeyName) &&
		StatePropertyReference.ExpectedValueType != EStatePropertyType::Invalid &&
		StatePropertyReference.ExpectedValueType == GetTypeFromName(StatePropertyReference.StateKeyName);
}