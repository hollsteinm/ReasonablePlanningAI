// Troll Purse. All rights reserved.


#include "Planners/RpaiPlanner_HUG.h"
#include "Core/RpaiGoalBase.h"
#include "Core/RpaiActionBase.h"
#include "Core/RpaiState.h"
#include "VisualLogger/VisualLoggerTypes.h"
#include "VisualLogger/VisualLogger.h"

URpaiPlanner_HUG::URpaiPlanner_HUG()
    : MaxIterations(250)
    , IterationsPerTick(50)
    , GoalDivergenceThreshold(0.1f)
{
    PlannerMemoryStructType = FHugPlannerMemory::StaticStruct();
}

ERpaiPlannerResult URpaiPlanner_HUG::ReceiveStartGoalPlanning_Implementation(
    const URpaiGoalBase* TargetGoal,
    const URpaiState* CurrentState,
    const TArray<URpaiActionBase*>& GivenActions,
    TArray<URpaiActionBase*>& OutActions,
    FRpaiMemoryStruct PlannerMemory
) const
{
    FHugPlannerMemory* Memory = PlannerMemory.Get<FHugPlannerMemory>();
    Memory->OpenActions.Empty();
    Memory->ClosedActions.Empty();
    Memory->CurrentIterations = 0;
    Memory->FutureState = nullptr;
    Memory->CurrentState = nullptr;
    Memory->OriginalWeight = TargetGoal->GetWeight(CurrentState);

    if (TargetGoal->IsInDesiredState(CurrentState))
    {
        return ERpaiPlannerResult::CompletedSuccess;
    }
    Memory->CurrentState = NewObject<URpaiState>(GetOuter(), CurrentState->GetClass());

    FVisitedState Start;
    Start.Id = FGuid::NewGuid();
    Start.Action = nullptr;
    Start.Cost = 0.f;
    Start.Remaining = 0.f;
    Start.ParentId.Invalidate();
    Start.Snapshot(CurrentState);

    Memory->OpenActions.HeapPush(Start);
    Memory->FutureState = NewObject<URpaiState>(GetOuter(), CurrentState->GetClass());
    Memory->Scratch = NewObject<URpaiState>(GetOuter(), CurrentState->GetClass());
    return ERpaiPlannerResult::RequiresTick;
}

ERpaiPlannerResult URpaiPlanner_HUG::ReceiveTickGoalPlanning_Implementation(
    const URpaiGoalBase* TargetGoal,
    const URpaiState* CurrentState,
    const TArray<URpaiActionBase*>& GivenActions,
    TArray<URpaiActionBase*>& OutActions,
    FRpaiMemoryStruct PlannerMemory
) const
{
    FHugPlannerMemory* Memory = PlannerMemory.Get<FHugPlannerMemory>();
    int32 TickIterations = 0;

    while (Memory->OpenActions.Num() > 0 && ++(Memory->CurrentIterations) < MaxIterations && ++TickIterations < IterationsPerTick)
    {
        FVisitedState Current;
        Memory->OpenActions.HeapPop(Current);
        Memory->ClosedActions.Push(Current);
        Current.Materialize(Memory->CurrentState);
        if (TargetGoal->IsInDesiredState(Memory->CurrentState))
        {
            TArray<FVisitedState> Plan;
            Plan.Reserve(Memory->OpenActions.Num() + Memory->ClosedActions.Num());
            do
            {
                Plan.Push(Current);
                auto Next = Memory->OpenActions.FindByKey(Current.ParentId);
                if (Next == nullptr)
                {
                    Next = Memory->ClosedActions.FindByKey(Current.ParentId);
                }
                Current = *Next;
            } while (Current.ParentId.IsValid());

            // trim out tasks that breach divergence.
            for (const auto Task : Plan) {
                Task.Materialize(Memory->FutureState);
                const float Difference = TargetGoal->GetWeight(Memory->FutureState) - Memory->OriginalWeight;
                if (Difference > GoalDivergenceThreshold)
                {
                    break;
                }
                OutActions.Push(Task.Action);
            }

            // TODO Clean Memory
            return ERpaiPlannerResult::CompletedSuccess;
        }

        for (const auto& Action : GivenActions)
        {
            Current.Materialize(Memory->CurrentState);
            if (Action->IsApplicable(Memory->CurrentState))
            {
                Current.Materialize(Memory->FutureState);
                Action->ApplyToState(Memory->FutureState);

                if (Memory->ClosedActions.FindByPredicate([Memory](const FVisitedState& LHS) -> bool {
                    return MaterializeStateIsEqualToGivenState(Memory->Scratch, LHS, Memory->FutureState);
                    }) != nullptr)
                {
                    continue;
                }

                FVisitedState* InOpen = Memory->OpenActions.FindByPredicate([Memory](const FVisitedState& LHS) -> bool {
                    return MaterializeStateIsEqualToGivenState(Memory->Scratch, LHS, Memory->FutureState);
                });
                Current.Materialize(Memory->CurrentState);
                auto ActionCost = Action->ExecutionWeight(Memory->CurrentState);
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
                    NewNode.Snapshot(Memory->FutureState);

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
        //TODO clean memory
        return ERpaiPlannerResult::CompletedFailure;
    }
    return ERpaiPlannerResult::RequiresTick;
}

ERpaiPlannerResult URpaiPlanner_HUG::ReceiveCancelGoalPlanning_Implementation(
    const URpaiGoalBase* TargetGoal,
    const URpaiState* CurrentState,
    const TArray<URpaiActionBase*>& GivenActions,
    TArray<URpaiActionBase*>& OutActions,
    FRpaiMemoryStruct PlannerMemory
) const
{
    FHugPlannerMemory* Memory = PlannerMemory.Get<FHugPlannerMemory>();
    Memory->OpenActions.Empty();
    Memory->ClosedActions.Empty();
    return ERpaiPlannerResult::CompletedCancelled;
}

bool URpaiPlanner_HUG::ReceivePlanChosenGoal_Implementation(
    const URpaiGoalBase* TargetGoal,
    const URpaiState* CurrentState,
    const TArray<URpaiActionBase*>& GivenActions,
    TArray<URpaiActionBase*>& OutActions
) const
{
    FRpaiMemory InlineMemory(sizeof(FHugPlannerMemory));
    FRpaiMemoryStruct InlineMemoryStruct(&InlineMemory, FHugPlannerMemory::StaticStruct());
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
