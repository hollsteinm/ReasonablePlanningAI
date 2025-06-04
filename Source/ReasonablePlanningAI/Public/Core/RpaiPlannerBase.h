// Radaway Software LLC. 2025. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/RpaiTypes.h"
#include "RpaiPlannerBase.generated.h"

class URpaiGoalBase;
class URpaiActionBase;
class URpaiState;

/**
 * Class the implements the algorithm for planning a given goal, a current state, and a set of actions.
 */
UCLASS(BlueprintType, Blueprintable, Abstract, EditInlineNew, DefaultToInstanced)
class REASONABLEPLANNINGAI_API URpaiPlannerBase : public UObject
{
   GENERATED_BODY()
   
public:
   URpaiPlannerBase();

   /**
   * Given a target goal and current state, produce - if possible - a queue of actions to execute in insertion order. Returns false if no solution is found.
   */
   UFUNCTION(BlueprintPure, Category = "Rpai")
   bool PlanChosenGoal(const URpaiGoalBase* TargetGoal, const URpaiState* CurrentState, const TArray<URpaiActionBase*>& GivenActions, TArray<URpaiActionBase*>& OutActions) const;

   /**
   * The below functions enable AI Planning that takes multiple frames.
   */

   /**
   * Start planning the Goal. OutActions are usable if state is "CompletedSuccess". Call TickGoalPlanning with same parameters given to StartGoalPlanning to do multi-frame processing of goal planning if result is RequiresTick.
   */
   UFUNCTION(BlueprintPure, Category = "Rpai")
   ERpaiPlannerResult StartGoalPlanning(const URpaiGoalBase* TargetGoal, const URpaiState* CurrentState, const TArray<URpaiActionBase*>& GivenActions, TArray<URpaiActionBase*>& OutActions, FRpaiMemoryStruct PlannerMemory) const;

   /**
   * Only invoke after StartGoalPlanning or TickGoalPlanning and the result is RequiresTick.
   */
   UFUNCTION(BlueprintPure, Category = "Rpai")
   ERpaiPlannerResult TickGoalPlanning(const URpaiGoalBase* TargetGoal, const URpaiState* CurrentState, const TArray<URpaiActionBase*>& GivenActions, TArray<URpaiActionBase*>& OutActions, FRpaiMemoryStruct PlannerMemory) const;

   /**
   * Invoke only if calling StartGoalPlanning or TickGoalPlanning with same parameters of former functions and results were never any state other than RequiresTick. Intentionally cancel planning and clean up resources.
   **/
   UFUNCTION(BlueprintPure, Category = "Rpai")
   ERpaiPlannerResult CancelGoalPlanning(const URpaiGoalBase* TargetGoal, const URpaiState* CurrentState, const TArray<URpaiActionBase*>& GivenActions, TArray<URpaiActionBase*>& OutActions, FRpaiMemoryStruct PlannerMemory) const;

   /**
   * Create fly-weight memory to run multi-tick requests of this planner for multiple agents.
   **/
   UFUNCTION(BlueprintPure, Category = "Rpai")
   virtual FRpaiMemoryStruct AllocateMemorySlice(UPARAM(Ref) FRpaiMemory& FromMemory) const;

   /**
   * Write debug info strings
   **/
   virtual FString GetDebugInfoString(FRpaiMemoryStruct PlannerMemory) const { return TEXT(""); };

protected:
   /**
   * Struct used for runtime memory for each execution.
   **/
   UPROPERTY(EditDefaultsOnly, Category = "Rpai")
   UScriptStruct* PlannerMemoryStructType;

   /**
   * Override this to implement PlanChosenGoal
   */
   UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Rpai")
   bool ReceivePlanChosenGoal(const URpaiGoalBase* TargetGoal, const URpaiState* CurrentState, const TArray<URpaiActionBase*>& GivenActions, TArray<URpaiActionBase*>& OutActions) const;
   virtual bool ReceivePlanChosenGoal_Implementation(const URpaiGoalBase* TargetGoal, const URpaiState* CurrentState, const TArray<URpaiActionBase*>& GivenActions, TArray<URpaiActionBase*>& OutActions) const;

   /**
   * Override this to implement StartGoalPlanning
   */
   UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Rpai")
   ERpaiPlannerResult ReceiveStartGoalPlanning(const URpaiGoalBase* TargetGoal, const URpaiState* CurrentState, const TArray<URpaiActionBase*>& GivenActions, TArray<URpaiActionBase*>& OutActions, FRpaiMemoryStruct PlannerMemory) const;
   virtual ERpaiPlannerResult ReceiveStartGoalPlanning_Implementation(const URpaiGoalBase* TargetGoal, const URpaiState* CurrentState, const TArray<URpaiActionBase*>& GivenActions, TArray<URpaiActionBase*>& OutActions, FRpaiMemoryStruct PlannerMemory) const;

   /**
   * Override this to implement TickGoalPlanning
   */
   UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Rpai")
   ERpaiPlannerResult ReceiveTickGoalPlanning(const URpaiGoalBase* TargetGoal, const URpaiState* CurrentState, const TArray<URpaiActionBase*>& GivenActions, TArray<URpaiActionBase*>& OutActions, FRpaiMemoryStruct PlannerMemory) const;
   virtual ERpaiPlannerResult ReceiveTickGoalPlanning_Implementation(const URpaiGoalBase* TargetGoal, const URpaiState* CurrentState, const TArray<URpaiActionBase*>& GivenActions, TArray<URpaiActionBase*>& OutActions, FRpaiMemoryStruct PlannerMemory) const;

   /**
   * Override this to implement CancelGoalPlanning
   **/
   UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Rpai")
   ERpaiPlannerResult ReceiveCancelGoalPlanning(const URpaiGoalBase* TargetGoal, const URpaiState* CurrentState, const TArray<URpaiActionBase*>& GivenActions, TArray<URpaiActionBase*>& OutActions, FRpaiMemoryStruct PlannerMemory) const;
   virtual ERpaiPlannerResult ReceiveCancelGoalPlanning_Implementation(const URpaiGoalBase* TargetGoal, const URpaiState* CurrentState, const TArray<URpaiActionBase*>& GivenActions, TArray<URpaiActionBase*>& OutActions, FRpaiMemoryStruct PlannerMemory) const;
};
