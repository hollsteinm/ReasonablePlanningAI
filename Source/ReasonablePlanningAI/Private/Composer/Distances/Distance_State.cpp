// Troll Purse. All rights reserved.


#include "Composer/Distances/Distance_State.h"
#include "Core/ReasonablePlanningState.h"

//Return distance as the difference between the left hand property and the right hand property.
float UDistance_State::ReceiveCalculateDistance_Implementation(const UReasonablePlanningState* GivenState) const
{
    if (LHS.ExpectedValueType != RHS.ExpectedValueType)
    {
        UE_LOG(LogRPAI, Verbose, TEXT("Different expected types, returning 0"));
        return TNumericLimits<float>::Max();
    }

    if (LHS.StateKeyName == RHS.StateKeyName)
    {
        UE_LOG(LogRPAI, Verbose, TEXT("Same State Key referenced, returning 0"));
        return TNumericLimits<float>::Max();
    }

    if (!(GivenState->HasReferencedState(LHS) && GivenState->HasReferencedState(RHS)))
    {
        UE_LOG(LogRPAI, Warning, TEXT("Unable to calculate distance from %s to %s, state does not have both referenced keys. Types for LHS %s and RHS %s. For state %s"),
            *LHS.StateKeyName.ToString(),
            *RHS.StateKeyName.ToString(),
            *UEnum::GetValueAsName(LHS.ExpectedValueType).ToString(),
            *UEnum::GetValueAsName(RHS.ExpectedValueType).ToString(),
            *GivenState->GetName()
        );
        return 0.f;
    }

    switch (LHS.ExpectedValueType)
    {
    case EStatePropertyType::Bool:
    {
        bool bLHS = false, bRHS = false;
        if (GivenState->GetBool(LHS.StateKeyName, bLHS) && GivenState->GetBool(RHS.StateKeyName, bRHS))
        {
            return (bRHS == bLHS) ? 0.f : 1.f;
        }
        break;
    }
    case EStatePropertyType::Float:
    {
        float fLHS = 0.f, fRHS = 0.f;
        if (GivenState->GetFloat(LHS.StateKeyName, fLHS) && GivenState->GetFloat(RHS.StateKeyName, fRHS))
        {
            return fLHS - fRHS;
        }
        break;
    }
    case EStatePropertyType::Int:
    {
        int32 iLHS = 0, iRHS = 0;
        if (GivenState->GetInt(LHS.StateKeyName, iLHS) && GivenState->GetInt(RHS.StateKeyName, iRHS))
        {
            return iLHS - iRHS;
        }
        break;
    }
    case EStatePropertyType::Object:
    {
        UObject* uLHS = nullptr;
        UObject* uRHS = nullptr;
        if (GivenState->GetObject(LHS.StateKeyName, uLHS) && GivenState->GetObject(RHS.StateKeyName, uRHS))
        {
            AActor* aLHS = Cast<AActor>(uLHS);
            AActor* aRHS = Cast<AActor>(uRHS);
            return FVector::DistSquared(aLHS->GetActorLocation(), aRHS->GetActorLocation());

        }
        break;
    }
    case EStatePropertyType::Rotator:
    {
        FRotator rLHS, rRHS;
        if (GivenState->GetRotator(LHS.StateKeyName, rLHS) && GivenState->GetRotator(RHS.StateKeyName, rRHS))
        {
            return rLHS.GetManhattanDistance(rRHS);
        }
        break;
    }
    case EStatePropertyType::Vector:
    {
        FVector vLHS, vRHS;
        if (GivenState->GetVector(LHS.StateKeyName, vLHS) && GivenState->GetVector(RHS.StateKeyName, vRHS))
        {
            return FVector::DistSquared(vLHS, vRHS);
        }
        break;
    }
    case EStatePropertyType::Class:
    case EStatePropertyType::Enum:
    case EStatePropertyType::Name:
    case EStatePropertyType::String:
    case EStatePropertyType::Invalid:
    default:
        UE_LOG(LogRPAI, Warning, TEXT("Unable to calculate distance from %s to %s. Unhandled type LHS %s and RHS %s. For state %s"),
            *LHS.StateKeyName.ToString(),
            *RHS.StateKeyName.ToString(),
            *UEnum::GetValueAsName(LHS.ExpectedValueType).ToString(),
            *UEnum::GetValueAsName(RHS.ExpectedValueType).ToString(),
            *GivenState->GetName()
        );
        break;
    }

    return TNumericLimits<float>::Max();
}
