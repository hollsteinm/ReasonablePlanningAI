#include "BTTask/RpaiBTTask_DetermineNextGoal.h"
#include "Composer/RpaiComposerBehavior.h"
#include "Core/RpaiGoalBase.h"
#include "Core/RpaiReasonerBase.h"
#include "States/RpaiState_BlackboardComponentAdapter.h"
#include "VisualLogger/VisualLoggerTypes.h"
#include "VisualLogger/VisualLogger.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BTTask/RpaiStateBlackboardComponent.h"

URpaiBTTask_DetermineNextGoal::URpaiBTTask_DetermineNextGoal()
   : Reasoner(nullptr)
   , Goals()
{
   NodeName = "Determine Next Goal";
   BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(URpaiBTTask_DetermineNextGoal, BlackboardKey), URpaiGoalBase::StaticClass());
}

EBTNodeResult::Type URpaiBTTask_DetermineNextGoal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
   UE_VLOG(OwnerComp.GetOwner(), LogRpai, Log, TEXT("%s"), ANSI_TO_TCHAR(__FUNCTION__));
   check(Reasoner != nullptr);
   check(!Goals.IsEmpty());

   UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
   if (BlackboardComp == nullptr)
   {
      return EBTNodeResult::Failed;
   }

   URpaiStateBlackboardComponent* RpaiComp = OwnerComp.GetAIOwner()->FindComponentByClass<URpaiStateBlackboardComponent>();
   if (RpaiComp == nullptr)
   {
      UE_LOG(LogRpai, Error, TEXT("Missing URpaiStateBlackboardComponent"));
      return EBTNodeResult::Failed;
   }

   URpaiState_BlackboardComponentAdapter* BlackboardAdapterState = RpaiComp->GetStateAdapter();
   if (BlackboardAdapterState == nullptr)
   {
      UE_LOG(LogRpai, Error, TEXT("Missing URpaiStateBlackboardComponent::StateAdpater"));
      return EBTNodeResult::Failed;
   }

   BlackboardAdapterState->ReadFromBlackboard(BlackboardComp);
   URpaiGoalBase* NextGoal = Reasoner->ReasonNextGoal(Goals, BlackboardAdapterState);
   if (NextGoal == nullptr)
   {
      return EBTNodeResult::Failed;
   }

   BlackboardComp->SetValueAsObject(BlackboardKey.SelectedKeyName, NextGoal);
   return EBTNodeResult::Succeeded;
}

void URpaiBTTask_DetermineNextGoal::DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const
{
   Super::DescribeRuntimeValues(OwnerComp, NodeMemory, Verbosity, Values);

   const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
   if (BlackboardComp != nullptr)
   {
      const FString KeyValue = BlackboardComp->DescribeKeyValue(BlackboardKey.GetSelectedKeyID(), EBlackboardDescription::DetailedKeyWithValue);
      Values.Add(FString::Printf(TEXT("Current State: %s"), *KeyValue));

      const FString OutputKeyValue = BlackboardComp->DescribeKeyValue(BlackboardKey.GetSelectedKeyID(), EBlackboardDescription::DetailedKeyWithValue);
      Values.Add(FString::Printf(TEXT("Determined Goal: %s"), *OutputKeyValue));
   }
}

FString URpaiBTTask_DetermineNextGoal::GetStaticDescription() const
{
   FString KeyDesc("invalid");
   FString ReasonerDesc("invalid");

   if (BlackboardKey.IsSet())
   {
      KeyDesc = BlackboardKey.SelectedKeyName.ToString();
   }

   if (Reasoner != nullptr)
   {
      ReasonerDesc = Reasoner->GetName();
   }

   return FString::Printf(TEXT("%s: determine goal %s using reasoner %s"), *Super::GetStaticDescription(), *KeyDesc, *ReasonerDesc);
}

#if WITH_EDITOR
FName URpaiBTTask_DetermineNextGoal::GetNodeIconName() const
{
   return FName("BTEditor.Graph.BTNode.Task.RunBehavior.Icon");
}
#endif
