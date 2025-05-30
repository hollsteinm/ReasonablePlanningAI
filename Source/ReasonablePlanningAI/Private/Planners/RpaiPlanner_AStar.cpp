// Radaway Software LLC. 2025. All rights reserved.


#include "Planners/RpaiPlanner_AStar.h"
#include "Core/RpaiGoalBase.h"
#include "Core/RpaiActionBase.h"
#include "Core/RpaiState.h"
#include "VisualLogger/VisualLoggerTypes.h"
#include "VisualLogger/VisualLogger.h"

URpaiPlanner_AStar::URpaiPlanner_AStar()
    : MaxIterations(250)
    , IterationsPerTick(50)
{
    PlannerMemoryStructType = FAStarPlannerMemory::StaticStruct();
}

FString URpaiPlanner_AStar::GetDebugInfoString(FRpaiMemoryStruct PlannerMemory) const
{
    if (PlannerMemory.IsCompatibleType(PlannerMemoryStructType))
    {
        FAStarPlannerMemory* Memory = PlannerMemory.Get<FAStarPlannerMemory>();
        return FString::Printf(TEXT("A* Planner\n\tOpen Actions: %i\n\tClosed Actions: %i\n\tVisited States: %i\n"), Memory->OpenActions.Num(), Memory->ClosedActions.Num(), Memory->VisitedStates.Num());
    }
    else
    {
        return TEXT("Invalid Planner Memory Type!\n");
    }
}

ERpaiPlannerResult URpaiPlanner_AStar::ReceiveStartGoalPlanning_Implementation(
    const URpaiGoalBase* TargetGoal,
    const URpaiState* CurrentState,
    const TArray<URpaiActionBase*>& GivenActions,
    TArray<URpaiActionBase*>& OutActions,
    FRpaiMemoryStruct PlannerMemory
) const
{
    FAStarPlannerMemory* Memory = PlannerMemory.Get<FAStarPlannerMemory>();
    Memory->OpenActions.Empty();
    Memory->ClosedActions.Empty();
    Memory->VisitedStates.Empty();
    Memory->UnorderedNodes.Empty();
    Memory->CurrentIterations = 0;
    Memory->FutureState = nullptr;

    if (TargetGoal->IsInDesiredState(CurrentState))
    {
        return ERpaiPlannerResult::CompletedSuccess;
    }

    FVisitedState& Start = Memory->UnorderedNodes.AddDefaulted_GetRef();
    Start.SelfIndex = Memory->UnorderedNodes.Num() - 1;
    Start.Cost = 0.f;
    Start.Remaining = 0.f;
    Start.StateIndex = INDEX_NONE;
    Start.ParentIndex = INDEX_NONE;
    Start.Action = nullptr;

    const FText FormattedName = FText::Format(FText::FromString(FString("RootState_{0}")), FText::FromString(FGuid::NewGuid().ToString()));
    URpaiState* StartState = NewObject<URpaiState>(const_cast<URpaiPlanner_AStar*>(this), CurrentState->GetClass(), FName(FormattedName.ToString()));
    int32 RootIndex = Memory->VisitedStates.Add(StartState);

    Start.StateIndex = RootIndex;
    CurrentState->CopyStateForPredictionTo(StartState);

    Memory->OpenActions.HeapPush(Start);
    Memory->FutureState = NewObject<URpaiState>(const_cast<URpaiPlanner_AStar*>(this), CurrentState->GetClass(), TEXT("FutureState"));
    return ERpaiPlannerResult::RequiresTick;
}

ERpaiPlannerResult URpaiPlanner_AStar::ReceiveTickGoalPlanning_Implementation(
    const URpaiGoalBase* TargetGoal,
    const URpaiState* CurrentState,
    const TArray<URpaiActionBase*>& GivenActions,
    TArray<URpaiActionBase*>& OutActions,
    FRpaiMemoryStruct PlannerMemory
) const
{
    FAStarPlannerMemory* Memory = PlannerMemory.Get<FAStarPlannerMemory>();
    int32 TickIterations = 0;

    while (Memory->OpenActions.Num() > 0 && ++(Memory->CurrentIterations) < MaxIterations && ++TickIterations < IterationsPerTick)
    {
        FVisitedState Current;
        Memory->OpenActions.HeapPop(Current);
        Memory->ClosedActions.Push(Current);

        check(Memory->VisitedStates.IsValidIndex(Current.StateIndex));
        URpaiState* CurrentVisitedState = Memory->VisitedStates[Current.StateIndex];
        if (TargetGoal->IsInDesiredState(CurrentVisitedState))
        {
            for(;;)
            {
                OutActions.Push(Memory->UnorderedNodes[Current.SelfIndex].Action);
                if (Current.ParentIndex > 0 /*Ignore root node*/ && Memory->UnorderedNodes.IsValidIndex(Current.ParentIndex))
                {
                    Current = Memory->UnorderedNodes[Current.ParentIndex];
                }
                else
                {
                    break;
                }
            }
            CleanupInstanceMemory(Memory);
            return ERpaiPlannerResult::CompletedSuccess;
        }
        else
        {
            for (const auto& Action : GivenActions)
            {
                if (Action->IsApplicable(CurrentVisitedState))
                {
                    // GC Buster
                    if (Memory->FutureState->IsUnreachable())
                    {
                        Memory->FutureState = NewObject<URpaiState>(const_cast<URpaiPlanner_AStar*>(this), CurrentState->GetClass(), TEXT("FutureState"));
                    }
                    CurrentVisitedState->CopyStateForPredictionTo(Memory->FutureState);
                    Action->ApplyToState(Memory->FutureState);

                    if (Memory->ClosedActions.IsValidIndex(FindEqualNodeFromState(Memory->FutureState, Memory->VisitedStates, Memory->ClosedActions)))
                    {
                        continue;
                    }

                    auto ActionCost = Action->ExecutionWeight(CurrentVisitedState);
                    auto NewCost = Current.Cost + ActionCost;
                    auto NewRemaining = TargetGoal->GetDistanceToDesiredState(Memory->FutureState);

                    int32 OpenIndex = FindEqualNodeFromState(Memory->FutureState, Memory->VisitedStates, Memory->OpenActions);
                    if (!Memory->OpenActions.IsValidIndex(OpenIndex))
                    {
                        FVisitedState& NewNode = Memory->UnorderedNodes.AddDefaulted_GetRef();
                        NewNode.SelfIndex = Memory->UnorderedNodes.Num() - 1;
                        NewNode.Action = Action;
                        NewNode.Cost = NewCost;
                        NewNode.Remaining = NewRemaining;
                        NewNode.ParentIndex = Current.SelfIndex;

                        auto NewState = NewObject<URpaiState>(const_cast<URpaiPlanner_AStar*>(this), CurrentState->GetClass());
                        Memory->FutureState->CopyStateForPredictionTo(NewState);
                        NewNode.StateIndex = Memory->VisitedStates.Add(NewState);

                        // add the number of actions preceeding this action
                        Memory->OpenActions.HeapPush(NewNode);
                    }
                    else if (NewCost < Memory->OpenActions[OpenIndex].Cost)
                    {
                        Memory->OpenActions[OpenIndex].ParentIndex = Current.SelfIndex;
                        Memory->OpenActions[OpenIndex].Cost = NewCost;
                        Memory->OpenActions[OpenIndex].Action = Action;
                        Memory->OpenActions[OpenIndex].Remaining = NewRemaining;
                        Memory->OpenActions.HeapSort();
                    }
                }
            }
        }
    }

    if (Memory->CurrentIterations >= MaxIterations || Memory->OpenActions.IsEmpty())
    {
        CleanupInstanceMemory(Memory);
        return ERpaiPlannerResult::CompletedFailure;
    }
    return ERpaiPlannerResult::RequiresTick;
}

int32 URpaiPlanner_AStar::FindEqualNodeFromState(const URpaiState* Lookup, const TArray<TObjectPtr<URpaiState>>& States, const TArray<FVisitedState>& Nodes)
{
    for (int32 Idx = 0; Idx < Nodes.Num(); ++Idx)
    {
        auto NodeState = States[Nodes[Idx].StateIndex];
        if (Lookup->IsEqualTo(NodeState))
        {
            return Idx;
        }
    }
    return INDEX_NONE;
}

ERpaiPlannerResult URpaiPlanner_AStar::ReceiveCancelGoalPlanning_Implementation(
    const URpaiGoalBase* TargetGoal,
    const URpaiState* CurrentState,
    const TArray<URpaiActionBase*>& GivenActions,
    TArray<URpaiActionBase*>& OutActions,
    FRpaiMemoryStruct PlannerMemory
) const
{
    FAStarPlannerMemory* Memory = PlannerMemory.Get<FAStarPlannerMemory>();
    Memory->OpenActions.Empty();
    Memory->ClosedActions.Empty();
    return ERpaiPlannerResult::CompletedCancelled;
}

bool URpaiPlanner_AStar::ReceivePlanChosenGoal_Implementation(
    const URpaiGoalBase* TargetGoal,
    const URpaiState* CurrentState,
    const TArray<URpaiActionBase*>& GivenActions,
    TArray<URpaiActionBase*>& OutActions
) const
{
    FRpaiMemory InlineMemory(sizeof(FAStarPlannerMemory));
    FRpaiMemoryStruct InlineMemoryStruct(&InlineMemory, FAStarPlannerMemory::StaticStruct());
    ERpaiPlannerResult Result = StartGoalPlanning(
        TargetGoal,
        CurrentState,
        GivenActions,
        OutActions,
        InlineMemoryStruct
    );
    while (Result == ERpaiPlannerResult::RequiresTick)
    {
        Result = TickGoalPlanning(
            TargetGoal,
            CurrentState,
            GivenActions,
            OutActions,
            InlineMemoryStruct
        );
    }
    return Result == ERpaiPlannerResult::CompletedSuccess ? true : false;
}

void URpaiPlanner_AStar::CleanupInstanceMemory(FAStarPlannerMemory* Memory) const
{
    check(Memory != nullptr);
    if (IsValid(Memory->FutureState))
    {
        Memory->FutureState->ConditionalBeginDestroy();
    }

    for (auto& State : Memory->VisitedStates)
    {
        if (IsValid(State))
        {
            State->ConditionalBeginDestroy();
        }
    }

    Memory->OpenActions.Empty();
    Memory->ClosedActions.Empty();
    Memory->VisitedStates.Empty();
    Memory->UnorderedNodes.Empty();
}