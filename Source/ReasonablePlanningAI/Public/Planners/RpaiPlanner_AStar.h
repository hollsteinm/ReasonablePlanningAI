// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/RpaiPlannerBase.h"
#include "Core/RpaiState.h"
#include "RpaiPlanner_AStar.generated.h"

USTRUCT()
struct FVisitedStateMaterial
{
	GENERATED_BODY()

	UPROPERTY()
	FStateKeyValueReference Reference;

	TVariant<bool, UClass*, uint8, float, int32, FName, UObject*, FRotator, FVector, FString> CurrentValue;

	inline void Snapshot(const URpaiState* View)
	{
		switch (Reference.ExpectedValueType)
		{
		case EStatePropertyType::Bool:
		{
			bool Value;
			if (View->GetValueOfType(Reference.StateKeyName, Value))
			{
				CurrentValue.Set<bool>(Value);
			}
			else
			{
				UE_LOG(LogRpai, Warning, TEXT("Expected Value Type is not Bool"));
			}
			break;
		}
		case EStatePropertyType::Class:
		{
			UClass* Value;
			if (View->GetValueOfType(Reference.StateKeyName, Value))
			{
				CurrentValue.Set<UClass*>(Value);
			}
			else
			{
				UE_LOG(LogRpai, Warning, TEXT("Expected Value Type is not UClass"));
			}
			break;
		}
		case EStatePropertyType::Float:
		{
			float Value;
			if (View->GetValueOfType(Reference.StateKeyName, Value))
			{
				CurrentValue.Set<float>(Value);
			}
			else
			{
				UE_LOG(LogRpai, Warning, TEXT("Expected Value Type is not float"));
			}
			break;
		}
		case EStatePropertyType::Int:
		{
			int32 Value;
			if (View->GetValueOfType(Reference.StateKeyName, Value))
			{
				CurrentValue.Set<int32>(Value);
			}
			else
			{
				UE_LOG(LogRpai, Warning, TEXT("Expected Value Type is not Int"));
			}
			break;
		}
		case EStatePropertyType::Name:
		{
			FName Value;
			if (View->GetValueOfType(Reference.StateKeyName, Value))
			{
				CurrentValue.Set<FName>(Value);
			}
			else
			{
				UE_LOG(LogRpai, Warning, TEXT("Expected Value Type is not FName"));
			}
			break;
		}
		case EStatePropertyType::Object:
		{
			UObject* Value;
			if (View->GetValueOfType(Reference.StateKeyName, Value))
			{
				CurrentValue.Set<UObject*>(Value);
			}
			else
			{
				UE_LOG(LogRpai, Warning, TEXT("Expected Value Type is not UObject"));
			}
			break;
		}
		case EStatePropertyType::Rotator:
		{
			FRotator Value;
			if (View->GetValueOfType(Reference.StateKeyName, Value))
			{
				CurrentValue.Set<FRotator>(Value);
			}
			else
			{
				UE_LOG(LogRpai, Warning, TEXT("Expected Value Type is not FRotator"));
			}
			break;
		}
		case EStatePropertyType::String:
		{
			FString Value;
			if (View->GetValueOfType(Reference.StateKeyName, Value))
			{
				CurrentValue.Set<FString>(Value);
			}
			else
			{
				UE_LOG(LogRpai, Warning, TEXT("Expected Value Type is not FString"));
			}
			break;
		}
		case EStatePropertyType::Vector:
		{
			FVector Value;
			if (View->GetValueOfType(Reference.StateKeyName, Value))
			{
				CurrentValue.Set<FVector>(Value);
			}
			else
			{
				UE_LOG(LogRpai, Warning, TEXT("Expected Value Type is not FVector"));
			}
			break;
		}
		case EStatePropertyType::Enum:
		case EStatePropertyType::Invalid:
		default:
			UE_LOG(LogRpai, Warning, TEXT("Expected Value Type invalid or unaccounted"));
			break;
		}
	}
	
	inline void Apply(URpaiState* View) const
	{
		switch (Reference.ExpectedValueType)
		{
		case EStatePropertyType::Bool:
			View->SetValueOfType(Reference.StateKeyName, CurrentValue.Get<bool>());
			break;
		case EStatePropertyType::Class:
			View->SetValueOfType(Reference.StateKeyName, CurrentValue.Get<UClass*>());
			break;
		case EStatePropertyType::Float:
			View->SetValueOfType(Reference.StateKeyName, CurrentValue.Get<float>());
			break;
		case EStatePropertyType::Int:
			View->SetValueOfType(Reference.StateKeyName, CurrentValue.Get<int32>());
			break;
		case EStatePropertyType::Name:
			View->SetValueOfType(Reference.StateKeyName, CurrentValue.Get<FName>());
			break;
		case EStatePropertyType::Object:
			View->SetValueOfType(Reference.StateKeyName, CurrentValue.Get<UObject*>());
			break;
		case EStatePropertyType::Rotator:
			View->SetValueOfType(Reference.StateKeyName, CurrentValue.Get<FRotator>());
			break;
		case EStatePropertyType::String:
			View->SetValueOfType(Reference.StateKeyName, CurrentValue.Get<FString>());
			break;
		case EStatePropertyType::Vector:
			View->SetValueOfType(Reference.StateKeyName, CurrentValue.Get<FVector>());
			break;
		case EStatePropertyType::Enum:
		case EStatePropertyType::Invalid:
		default:
			UE_LOG(LogRpai, Warning, TEXT("Expected Value Type invalid or unaccounted"));
			break;
		}
	}
};

USTRUCT()
struct FVisitedState
{
	GENERATED_BODY()

	FGuid Id; // identifies the Action as a unique execution of the action
	FGuid ParentId; // the preceeding action that must occur
	float Cost; // calculated effort to perform the action
	float Remaining; // distance left to accomplish the goal

	// When iterated upon and applied to the values of a state, creates a materialized view 
	// of what the state would be if this visited state is applied.
	UPROPERTY()
	TArray<FVisitedStateMaterial> Views;

	UPROPERTY()
	URpaiActionBase* Action; //non-unique action performed to reach the above state

	inline void Materialize(URpaiState* State) const
	{
		for (const auto& View : Views)
		{
			View.Apply(State);
		}
	}

	inline void Snapshot(const URpaiState* State)
	{
		TArray<FStateKeyValueReference> References;
		State->GatherStateKeyReferences(References);
		Views.Empty(References.Num());
		for (const auto& Reference : References)
		{
			FVisitedStateMaterial Material;
			Material.Reference = Reference;
			Material.Snapshot(State);
			Views.Add(Material);
		}
	}
};

inline bool operator==(const FVisitedState& LHS, const FVisitedState& RHS)
{
	return LHS.Id == RHS.Id;
}

static bool MaterializeStateIsEqualToGivenState(URpaiState* Scratch, const FVisitedState& LHS, URpaiState* RHS)
{
	check(RHS != nullptr);
	check(Scratch != nullptr);
	LHS.Materialize(Scratch);
	return Scratch->IsEqualTo(RHS);
}

inline bool operator==(const FVisitedState& LHS, const FGuid& RHS)
{
	return LHS.Id == RHS;
}

inline bool operator<(const FVisitedState& LHS, const FVisitedState& RHS)
{
	return LHS.Cost + LHS.Remaining < RHS.Cost + RHS.Remaining;
}

USTRUCT()
struct FAStarPlannerMemory
{
	GENERATED_BODY()

	TArray<FVisitedState> OpenActions; // all of the open actions to be explored
	TArray<FVisitedState> ClosedActions; // all of the closed actions terminating
	int32 CurrentIterations; // used the track the number of executions to plan

	UPROPERTY()
	URpaiState* FutureState; // cached state scratch pad for projection

	UPROPERTY()
	URpaiState* CurrentState; // used as the root for new Objects

	UPROPERTY()
	URpaiState* Scratch; // for lookups
};

/**
 * Uses a simple A-Star algorithm to select from the list of given actions to accomplish
 * the given goal based on the current state.
 */
UCLASS()
class REASONABLEPLANNINGAI_API URpaiPlanner_AStar : public URpaiPlannerBase
{
	GENERATED_BODY()

public:
	URpaiPlanner_AStar();

protected:
	// Used as a method to control planning so we don't go on an exhaustive journey
	UPROPERTY(EditAnywhere, Category = Rpai)
	int32 MaxIterations;

	// Number of iterations to perform per tick until a plan is solved or MaxIterations is capped
	UPROPERTY(EditAnywhere, Category = Rpai)
	int32 IterationsPerTick;

	virtual bool ReceivePlanChosenGoal_Implementation(
		const URpaiGoalBase* TargetGoal,
		const URpaiState* CurrentState,
		const TArray<URpaiActionBase*>& GivenActions,
		TArray<URpaiActionBase*>& OutActions
	) const override;

	virtual ERpaiPlannerResult ReceiveStartGoalPlanning_Implementation(
		const URpaiGoalBase* TargetGoal,
		const URpaiState* CurrentState,
		const TArray<URpaiActionBase*>& GivenActions,
		TArray<URpaiActionBase*>& OutActions,
		FRpaiMemoryStruct PlannerMemory
	) const override;

	virtual ERpaiPlannerResult ReceiveTickGoalPlanning_Implementation(
		const URpaiGoalBase* TargetGoal,
		const URpaiState* CurrentState,
		const TArray<URpaiActionBase*>& GivenActions,
		TArray<URpaiActionBase*>& OutActions,
		FRpaiMemoryStruct PlannerMemory
	) const override;

	virtual ERpaiPlannerResult ReceiveCancelGoalPlanning_Implementation(
		const URpaiGoalBase* TargetGoal,
		const URpaiState* CurrentState,
		const TArray<URpaiActionBase*>& GivenActions,
		TArray<URpaiActionBase*>& OutActions,
		FRpaiMemoryStruct PlannerMemory
	) const override;
};
