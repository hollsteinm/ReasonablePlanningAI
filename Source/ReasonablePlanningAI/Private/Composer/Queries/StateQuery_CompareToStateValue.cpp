// Troll Purse. All rights reserved.


#include "Composer/Queries/StateQuery_CompareToStateValue.h"
#include "Core/ReasonablePlanningState.h"

EStateQueryResult UStateQuery_CompareToStateValue::Query(const UReasonablePlanningState* StateToQuery) const
{
    if(StateToQuery == nullptr)
    {
        return EStateQueryResult::Invalid;
    }
    
    if(!StateToQuery->HasReferencedState(QueriedState))
    {
        return EStateQueryResult::Invalid;
    }
    
    if(!StateToQuery->HasReferencedState(ComparedToState))
    {
        return EStateQueryResult::Invalid;
    }
    
    if(ComparedToState.ExpectedValueType != QueriedState.ExpectedValueType)
    {
        return EStateQueryResult::Invalid;
    }
    
    switch (QueriedState.ExpectedValueType)
    {
    case EStatePropertyType::Bool:
    {
        bool bLHS = false, bRHS = false;
        if (StateToQuery->GetBool(QueriedState.StateKeyName, bLHS) && StateToQuery->GetBool(ComparedToState.StateKeyName, bRHS))
        {
            return DoCompare(ComparisonOperation, bLHS, bRHS);
        }
        break;
    }
    case EStatePropertyType::Float:
    {
        float fLHS = 0.f, fRHS = 0.f;
        if (StateToQuery->GetFloat(QueriedState.StateKeyName, fLHS) && StateToQuery->GetFloat(ComparedToState.StateKeyName, fRHS))
        {
            return DoCompare(ComparisonOperation, fLHS, fRHS);
        }
        break;
    }
    case EStatePropertyType::Int:
    {
        int32 iLHS = 0, iRHS = 0;
        if (StateToQuery->GetInt(QueriedState.StateKeyName, iLHS) && StateToQuery->GetInt(ComparedToState.StateKeyName, iRHS))
        {
            return DoCompare(ComparisonOperation, iLHS, iRHS);
        }
        break;
    }

    case EStatePropertyType::Rotator:
    {
        FRotator rLHS, rRHS;
        if (StateToQuery->GetRotator(QueriedState.StateKeyName, rLHS) && StateToQuery->GetRotator(ComparedToState.StateKeyName, rRHS))
        {
            return DoCompare(ComparisonOperation, rLHS, rRHS);
        }
        break;
    }
    case EStatePropertyType::Vector:
    {
        FVector vLHS, vRHS;
        if (StateToQuery->GetVector(QueriedState.StateKeyName, vLHS) && StateToQuery->GetVector(ComparedToState.StateKeyName, vRHS))
        {
            return DoCompare(ComparisonOperation, vLHS, vRHS);
        }
        break;
    }
    case EStatePropertyType::String:
    {
        FString sLHS, sRHS;
        if (StateToQuery->GetString(QueriedState.StateKeyName, sLHS) && StateToQuery->GetString(ComparedToState.StateKeyName, sRHS))
        {
            return DoCompare(ComparisonOperation, sLHS, sRHS);
        }
        break;
    }
    case EStatePropertyType::Object:
    case EStatePropertyType::Class:
    case EStatePropertyType::Enum:
    case EStatePropertyType::Name:
    case EStatePropertyType::Invalid:
    default:
        UE_LOG(LogRPAI, Warning, TEXT("Unable to compare %s to %s. Unhandled type LHS %s and RHS %s. For state %s"),
            *QueriedState.StateKeyName.ToString(),
            *ComparedToState.StateKeyName.ToString(),
            *UEnum::GetValueAsName(QueriedState.ExpectedValueType).ToString(),
            *UEnum::GetValueAsName(ComparedToState.ExpectedValueType).ToString(),
            *StateToQuery->GetName()
        );
        break;
    }
    
    return EStateQueryResult::Invalid;
}
