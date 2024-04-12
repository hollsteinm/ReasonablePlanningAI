// Troll Purse. All rights reserved.


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
    Memory->CurrentIterations = 0;
    Memory->FutureState = nullptr;
    Memory->DisposableRoot = nullptr;

    if (TargetGoal->IsInDesiredState(CurrentState))
    {
        return ERpaiPlannerResult::CompletedSuccess;
    }
    Memory->DisposableRoot = NewObject<UObject>(GetTransientPackage(), CurrentState->GetClass());

    FVisitedState Start;
    Start.Id = FGuid::NewGuid();
    Start.Action = nullptr;
    Start.Cost = 0.f;
    Start.Remaining = 0.f;
    Start.ParentId.Invalidate();
    Start.State = NewObject<URpaiState>(Memory->DisposableRoot, CurrentState->GetClass());

    CurrentState->CopyStateForPredictionTo(Start.State);

    Memory->OpenActions.HeapPush(Start);
    Memory->FutureState = NewObject<URpaiState>(Memory->DisposableRoot, CurrentState->GetClass());
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

        if (TargetGoal->IsInDesiredState(Current.State))
        {
            do
            {
                OutActions.Push(Current.Action);
                auto Next = Memory->OpenActions.FindByKey(Current.ParentId);
                if (Next == nullptr)
                {
                    Next = Memory->ClosedActions.FindByKey(Current.ParentId);
                }
                Current = *Next;
            } while (Current.ParentId.IsValid());
            Memory->DisposableRoot->ConditionalBeginDestroy();
            return ERpaiPlannerResult::CompletedSuccess;
        }

        for (const auto& Action : GivenActions)
        {
            if (Action->IsApplicable(Current.State))
            {
                Current.State->CopyStateForPredictionTo(Memory->FutureState);
                Action->ApplyToState(Memory->FutureState);

                if (Memory->ClosedActions.FindByKey(Memory->FutureState) != nullptr)
                {
                    continue;
                }

                FVisitedState* InOpen = Memory->OpenActions.FindByKey(Memory->FutureState);
                auto ActionCost = Action->ExecutionWeight(Current.State);
                auto NewCost = Current.Cost + ActionCost;
                auto NewRemaining = TargetGoal->GetDistanceToDesiredState(Memory->FutureState);

                if (InOpen == nullptr)
                {
                    FVisitedState NewNode;
                    NewNode.Id = FGuid::NewGuid();
                    NewNode.Action = Action;
                    NewNode.Cost = NewCost;
                    NewNode.Remaining = NewRemaining;
                    NewNode.ParentId = Current.Id;
                    NewNode.State = NewObject<URpaiState>(Memory->DisposableRoot, CurrentState->GetClass());
                    Memory->FutureState->CopyStateForPredictionTo(NewNode.State);

                    Memory->OpenActions.HeapPush(NewNode);
                }
                else
                {
                    if (NewCost < InOpen->Cost)
                    {
                        InOpen->ParentId = Current.Id;
                        InOpen->Cost = NewCost;
                        InOpen->Action = Action;
                        InOpen->Remaining = NewRemaining;

                        Memory->OpenActions.HeapSort();
                    }
                }
            }
        }
    }

    if (Memory->CurrentIterations >= MaxIterations || Memory->OpenActions.IsEmpty())
    {
        Memory->DisposableRoot->ConditionalBeginDestroy();
        return ERpaiPlannerResult::CompletedFailure;
    }
    return ERpaiPlannerResult::RequiresTick;
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
