// Troll Purse. All rights reserved.


#include "Planners/RpaiPlanner_HUG.h"
#include "Core/RpaiGoalBase.h"
#include "Core/RpaiActionBase.h"
#include "Core/RpaiState.h"
#include "VisualLogger/VisualLoggerTypes.h"
#include "VisualLogger/VisualLogger.h"

struct FHugVisitedState
{
    FGuid Id;
    FGuid ParentId;

    float Cost;
    float Remaining;

    URpaiState* State;
    URpaiActionBase* Action;
};

bool operator==(const FHugVisitedState& LHS, const FHugVisitedState& RHS)
{
    return LHS.Id == RHS.Id;
}

bool operator==(const FHugVisitedState& LHS, URpaiState* RHS)
{
    check(LHS.State != nullptr);
    check(RHS != nullptr);
    return LHS.State->IsEqualTo(RHS);
}

bool operator==(const FHugVisitedState& LHS, const FGuid& RHS)
{
    return LHS.Id == RHS;
}

bool operator<(const FHugVisitedState& LHS, const FHugVisitedState& RHS)
{
    return LHS.Cost + LHS.Remaining < RHS.Cost + RHS.Remaining;
}


URpaiPlanner_HUG::URpaiPlanner_HUG()
    : MaxPlanningAttemptThreshold(1000U)
    , GoalDivergenceThreshold(0.01f)
{

}

bool URpaiPlanner_HUG::ReceivePlanChosenGoal_Implementation(const URpaiGoalBase* TargetGoal, const URpaiState* CurrentState, const TArray<URpaiActionBase*>& GivenActions, TArray<URpaiActionBase*>& OutActions) const
{
    if (TargetGoal->IsInDesiredState(CurrentState))
    {
        return true; //or false?
    }

    TArray<FHugVisitedState> OpenActions;
    TArray<FHugVisitedState> ClosedActions;

    FHugVisitedState Start;
    Start.Id = FGuid::NewGuid();
    Start.Action = nullptr;
    Start.Cost = 0.f;
    Start.Remaining = 0.f;
    Start.ParentId.Invalidate();
    Start.State = NewObject<URpaiState>(GetTransientPackage(), CurrentState->GetClass());

    CurrentState->CopyStateForPredictionTo(Start.State);

    OpenActions.HeapPush(Start);
    uint32 Iterations = 0;
    //Reusable so we are only instantiatng new states when we need to
    URpaiState* FutureState = NewObject<URpaiState>(Start.State, CurrentState->GetClass());

    while (OpenActions.Num() > 0 && ++Iterations < MaxPlanningAttemptThreshold)
    {
        FHugVisitedState Current;
        OpenActions.HeapPop(Current);
        ClosedActions.Push(Current);

        if (TargetGoal->IsInDesiredState(Current.State))
        {
            do
            {
                OutActions.Push(Current.Action);
                auto Next = OpenActions.FindByKey(Current.ParentId);
                if (Next == nullptr)
                {
                    Next = ClosedActions.FindByKey(Current.ParentId);
                }
                Current = *Next;
            } while (Current.ParentId.IsValid() && FMath::Abs(TargetGoal->GetWeight(Current.State)) < GoalDivergenceThreshold);
            Start.State->ConditionalBeginDestroy();
            return true;
        }

        for (const auto& Action : GivenActions)
        {
            if (Action->IsApplicable(Current.State))
            {
                Current.State->CopyStateForPredictionTo(FutureState);
                Action->ApplyToState(FutureState);

                if (ClosedActions.FindByKey(FutureState) != nullptr)
                {
                    continue;
                }

                FHugVisitedState* InOpen = OpenActions.FindByKey(FutureState);
                auto ActionCost = Action->ExecutionWeight(Current.State);
                auto NewCost = Current.Cost + ActionCost;
                auto NewRemaining = TargetGoal->GetDistanceToDesiredState(FutureState);

                if (InOpen == nullptr)
                {
                    FHugVisitedState NewNode;
                    NewNode.Id = FGuid::NewGuid();
                    NewNode.Action = Action;
                    NewNode.Cost = NewCost;
                    NewNode.Remaining = NewRemaining;
                    NewNode.ParentId = Current.Id;
                    NewNode.State = NewObject<URpaiState>(Start.State, CurrentState->GetClass());
                    FutureState->CopyStateForPredictionTo(NewNode.State);

                    OpenActions.HeapPush(NewNode);
                }
                else
                {
                    if (NewCost < InOpen->Cost)
                    {
                        InOpen->ParentId = Current.Id;
                        InOpen->Cost = NewCost;
                        InOpen->Action = Action;
                        InOpen->Remaining = NewRemaining;

                        OpenActions.HeapSort();
                    }
                }
            }
        }
    }
    Start.State->ConditionalBeginDestroy();
    return false;
}
