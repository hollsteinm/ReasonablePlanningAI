// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.

#include "ComposerActionTaskTests.h"
#include "Misc/AutomationTest.h"
#include "AIController.h"
#include "Composer/ActionTasks/RpaiActionTask_Sequence.h"
#include "Composer/ActionTasks/RpaiActionTask_Composite.h"


UTestComposerActionTask::UTestComposerActionTask()
   : UpdateCallsUntilCompletion(2U)
{
   bCompleteAfterStart = false;
}

void UTestComposerActionTask::ReceiveStartActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld)
{
   ++StartActionTaskCallsTotal;
}

void UTestComposerActionTask::ReceiveUpdateActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, float DeltaSeconds, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld)
{
   ++UpdateActionTaskCallsTotal;
   if (UpdateActionTaskCallsTotal >= UpdateCallsUntilCompletion)
   {
      CompleteActionTask(ActionInstigator, CurrentState, ActionMemory, ActionTargetActor, ActionWorld);
   }
}

void UTestComposerActionTask::ReceiveCancelActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld, bool bCancelShouldExitPlan)
{
   ++CancelActionTaskCallsTotal;
}

void UTestComposerActionTask::ReceiveCompleteActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor, UWorld* ActionWorld)
{
   ++CompleteActionTaskCallsTotal;
}

BEGIN_DEFINE_SPEC(ReasonablePlanningComposerActionTaskSequenceSpec, "ReasonablePlanningAI.Composer.ActionTask", EAutomationTestFlags::ProductFilter | EAutomationTestFlags_ApplicationContextMask)
   AAIController* GivenController;
   URpaiActionTask_Sequence* ActionTaskUnderTest;
   FRpaiMemory GivenPool;
   FRpaiMemoryStruct GivenTestMemory;
END_DEFINE_SPEC(ReasonablePlanningComposerActionTaskSequenceSpec)
void ReasonablePlanningComposerActionTaskSequenceSpec::Define()
{
   Describe("Sequence", [this]()
      {
         BeforeEach([this]()
            {
               GivenController = NewObject<AAIController>();
               ActionTaskUnderTest = NewObject<URpaiActionTask_Sequence>();
               ActionTaskUnderTest->AddActionTaskToSequence(NewObject<UTestComposerActionTask>(ActionTaskUnderTest));
               ActionTaskUnderTest->AddActionTaskToSequence(NewObject<UTestComposerActionTask>(ActionTaskUnderTest));
               ActionTaskUnderTest->AddActionTaskToSequence(NewObject<UTestComposerActionTask>(ActionTaskUnderTest));
               ActionTaskUnderTest->AddActionTaskToSequence(NewObject<UTestComposerActionTask>(ActionTaskUnderTest));
               ActionTaskUnderTest->AddActionTaskToSequence(NewObject<UTestComposerActionTask>(ActionTaskUnderTest));

               GivenPool = FRpaiMemory(512);
               GivenTestMemory = ActionTaskUnderTest->AllocateMemorySlice(GivenPool);

               ActionTaskUnderTest->StartActionTask(GivenController, nullptr, GivenTestMemory);
            });

         It("should call the start method of the first sequence item", [this]()
            {
               TestTrue("Start Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromSequence(0))->WasStartActionTaskCalledNumTimes(1));
               TestTrue("Start Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromSequence(1))->WasStartActionTaskCalledNever());
               TestTrue("Start Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromSequence(2))->WasStartActionTaskCalledNever());
               TestTrue("Start Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromSequence(3))->WasStartActionTaskCalledNever());
               TestTrue("Start Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromSequence(4))->WasStartActionTaskCalledNever());
            });

         It("should call the update method of the first sequence item", [this]()
            {
               ActionTaskUnderTest->UpdateActionTask(GivenController, nullptr, 0.33f, GivenTestMemory);

               TestTrue("Update Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromSequence(0))->WasUpdateActionTaskCalledNumTimes(1));
               TestTrue("Update Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromSequence(1))->WasUpdateActionTaskCalledNever());
               TestTrue("Update Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromSequence(2))->WasUpdateActionTaskCalledNever());
               TestTrue("Update Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromSequence(3))->WasUpdateActionTaskCalledNever());
               TestTrue("Update Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromSequence(4))->WasUpdateActionTaskCalledNever());
            });

         It("should call the update method of the second sequnce item after update completes on first item", [this]()
            {
               ActionTaskUnderTest->UpdateActionTask(GivenController, nullptr, 0.33f, GivenTestMemory);
               ActionTaskUnderTest->UpdateActionTask(GivenController, nullptr, 0.33f, GivenTestMemory);

               TestTrue("Update Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromSequence(0))->WasUpdateActionTaskCalledNumTimes(2));
               TestTrue("Update Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromSequence(1))->WasUpdateActionTaskCalledNever());
               TestTrue("Update Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromSequence(2))->WasUpdateActionTaskCalledNever());
               TestTrue("Update Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromSequence(3))->WasUpdateActionTaskCalledNever());
               TestTrue("Update Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromSequence(4))->WasUpdateActionTaskCalledNever());

               TestTrue("Copmlete Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromSequence(0))->WasCompleteActionTaskCalledNumTimes(1));
               TestTrue("Complete Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromSequence(1))->WasCompleteActionTaskCalledNever());
               TestTrue("Start Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromSequence(1))->WasStartActionTaskCalledNumTimes(1));
               TestTrue("Complete Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromSequence(2))->WasCompleteActionTaskCalledNever());
               TestTrue("Complete Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromSequence(3))->WasCompleteActionTaskCalledNever());
               TestTrue("Complete Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromSequence(4))->WasCompleteActionTaskCalledNever());

               ActionTaskUnderTest->UpdateActionTask(GivenController, nullptr, 0.33f, GivenTestMemory);

               TestTrue("Update Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromSequence(0))->WasUpdateActionTaskCalledNumTimes(2));
               TestTrue("Update Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromSequence(1))->WasUpdateActionTaskCalledNumTimes(1));
               TestTrue("Update Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromSequence(2))->WasUpdateActionTaskCalledNever());
               TestTrue("Update Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromSequence(3))->WasUpdateActionTaskCalledNever());
               TestTrue("Update Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromSequence(4))->WasUpdateActionTaskCalledNever());
            });

         It("should call the complete method of the sequence when all tasks have completed", [this]()
            {
               uint32 CompletionCalls = 0U;
               ActionTaskUnderTest->OnActionTaskComplete().AddLambda([&CompletionCalls](URpaiComposerActionTaskBase*, AAIController*, URpaiState*) -> void { ++CompletionCalls; });

               for (auto Idx = 0; Idx < 10; ++Idx)
               {
                  ActionTaskUnderTest->UpdateActionTask(GivenController, nullptr, 0.33f, GivenTestMemory);
               }

               TestTrue("Update Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromSequence(0))->WasUpdateActionTaskCalledNumTimes(2));
               TestTrue("Update Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromSequence(1))->WasUpdateActionTaskCalledNumTimes(2));
               TestTrue("Update Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromSequence(2))->WasUpdateActionTaskCalledNumTimes(2));
               TestTrue("Update Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromSequence(3))->WasUpdateActionTaskCalledNumTimes(2));
               TestTrue("Update Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromSequence(4))->WasUpdateActionTaskCalledNumTimes(2));

               TestTrue("Complete Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromSequence(0))->WasCompleteActionTaskCalledNumTimes(1));
               TestTrue("Complete Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromSequence(1))->WasCompleteActionTaskCalledNumTimes(1));
               TestTrue("Complete Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromSequence(2))->WasCompleteActionTaskCalledNumTimes(1));
               TestTrue("Complete Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromSequence(3))->WasCompleteActionTaskCalledNumTimes(1));
               TestTrue("Complete Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromSequence(4))->WasCompleteActionTaskCalledNumTimes(1));

               TestEqual("Complete Called Once", CompletionCalls, 1U);
            });

         It("should only call the cancel action task function on the current task.", [this]()
            {
               uint32 CancelCalls = 0U;
               ActionTaskUnderTest->OnActionTaskCancelled().AddLambda([&CancelCalls](URpaiComposerActionTaskBase*, AAIController*, URpaiState*, bool) -> void { ++CancelCalls; });
               ActionTaskUnderTest->CancelActionTask(GivenController, nullptr, GivenTestMemory);

               TestTrue("Cancel Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromSequence(0))->WasCancelActionTaskCalledNumTimes(1));
               TestTrue("Cancel Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromSequence(1))->WasCancelActionTaskCalledNever());
               TestTrue("Cancel Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromSequence(2))->WasCancelActionTaskCalledNever());
               TestTrue("Cancel Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromSequence(3))->WasCancelActionTaskCalledNever());
               TestTrue("Cancel Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromSequence(4))->WasCancelActionTaskCalledNever());

               TestEqual("Cancel Called Once", CancelCalls, 1U);
            });

         It("should only call the cancel action task function on the current task after updates.", [this]()
            {
               uint32 CancelCalls = 0U;
               ActionTaskUnderTest->OnActionTaskCancelled().AddLambda([&CancelCalls](URpaiComposerActionTaskBase*, AAIController*, URpaiState*, bool) -> void { ++CancelCalls; });
               
               for (auto Idx = 0; Idx < 4; ++Idx)
               {
                  ActionTaskUnderTest->UpdateActionTask(GivenController, nullptr, 0.33f, GivenTestMemory);
               }
               ActionTaskUnderTest->CancelActionTask(GivenController, nullptr, GivenTestMemory);

               TestTrue("Cancel Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromSequence(0))->WasCancelActionTaskCalledNever());
               TestTrue("Cancel Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromSequence(1))->WasCancelActionTaskCalledNever());
               TestTrue("Cancel Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromSequence(2))->WasCancelActionTaskCalledNumTimes(1));
               TestTrue("Cancel Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromSequence(3))->WasCancelActionTaskCalledNever());
               TestTrue("Cancel Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromSequence(4))->WasCancelActionTaskCalledNever());

               TestEqual("Cancel Called Once", CancelCalls, 1U);
            });

         AfterEach([this]()
            {
               GivenController->Destroy();
               ActionTaskUnderTest->ConditionalBeginDestroy();
            });
      });
}

BEGIN_DEFINE_SPEC(ReasonablePlanningComposerActionTaskComposerSpec, "ReasonablePlanningAI.Composer.ActionTask", EAutomationTestFlags::ProductFilter | EAutomationTestFlags_ApplicationContextMask)
   AAIController* GivenController;
   URpaiActionTask_Composite* ActionTaskUnderTest;
   FRpaiMemory GivenPool;
   FRpaiMemoryStruct GivenMemory;
END_DEFINE_SPEC(ReasonablePlanningComposerActionTaskComposerSpec)
void ReasonablePlanningComposerActionTaskComposerSpec::Define()
{
   Describe("Composite", [this]()
      {
         BeforeEach([this]()
            {
               GivenController = NewObject<AAIController>();
               ActionTaskUnderTest = NewObject<URpaiActionTask_Composite>();

               
               FRpaiCompositeActionTaskEntry Entry;
               Entry.Action = NewObject<UTestComposerActionTask>();
               ActionTaskUnderTest->AddActionTaskToComposite(Entry);

               Entry.Action = NewObject<UTestComposerActionTask>();
               ActionTaskUnderTest->AddActionTaskToComposite(Entry);

               Entry.Action = NewObject<UTestComposerActionTask>();
               Entry.bIgnoredForCompositeCompletion = true;
               CastChecked<UTestComposerActionTask>(Entry.Action)->UpdateCallsUntilCompletion = 10;
               ActionTaskUnderTest->AddActionTaskToComposite(Entry);

               Entry.Action = NewObject<UTestComposerActionTask>();
               Entry.bPreferCancelOnCompositeCompletion = true;
               ActionTaskUnderTest->AddActionTaskToComposite(Entry);

               Entry.Action = NewObject<UTestComposerActionTask>();
               Entry.bIgnoredForCompositeCompletion = true;
               Entry.bPreferCancelOnCompositeCompletion = true;
               CastChecked<UTestComposerActionTask>(Entry.Action)->UpdateCallsUntilCompletion = 3;
               ActionTaskUnderTest->AddActionTaskToComposite(Entry);
               

               GivenPool = FRpaiMemory(512);
               GivenMemory = ActionTaskUnderTest->AllocateMemorySlice(GivenPool);

               ActionTaskUnderTest->StartActionTask(GivenController, nullptr, GivenMemory);
            });

         It("should call the start method of all entries", [this]()
            {
               TestTrue("Start Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromComposite(0))->WasStartActionTaskCalledNumTimes(1));
               TestTrue("Start Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromComposite(1))->WasStartActionTaskCalledNumTimes(1));
               TestTrue("Start Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromComposite(2))->WasStartActionTaskCalledNumTimes(1));
               TestTrue("Start Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromComposite(3))->WasStartActionTaskCalledNumTimes(1));
               TestTrue("Start Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromComposite(4))->WasStartActionTaskCalledNumTimes(1));
            });

         It("should call the update method of all entries", [this]()
            {
               ActionTaskUnderTest->UpdateActionTask(GivenController, nullptr, 0.33f, GivenMemory);

               TestTrue("Update Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromComposite(0))->WasUpdateActionTaskCalledNumTimes(1));
               TestTrue("Update Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromComposite(1))->WasUpdateActionTaskCalledNumTimes(1));
               TestTrue("Update Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromComposite(2))->WasUpdateActionTaskCalledNumTimes(1));
               TestTrue("Update Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromComposite(3))->WasUpdateActionTaskCalledNumTimes(1));
               TestTrue("Update Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromComposite(4))->WasUpdateActionTaskCalledNumTimes(1));
            });

         It("should call the update method of all entries, calls cancel if completing entry on behalf of composite runner", [this]()
            {
               ActionTaskUnderTest->UpdateActionTask(GivenController, nullptr, 0.33f, GivenMemory);
               ActionTaskUnderTest->UpdateActionTask(GivenController, nullptr, 0.33f, GivenMemory);

               TestTrue("Update Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromComposite(0))->WasUpdateActionTaskCalledNumTimes(2));
               TestTrue("Update Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromComposite(1))->WasUpdateActionTaskCalledNumTimes(2));
               TestTrue("Update Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromComposite(2))->WasUpdateActionTaskCalledNumTimes(2));
               TestTrue("Update Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromComposite(3))->WasUpdateActionTaskCalledNumTimes(2));
               TestTrue("Update Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromComposite(4))->WasUpdateActionTaskCalledNumTimes(2));

               TestTrue("Complete Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromComposite(0))->WasCompleteActionTaskCalledNumTimes(1));
               TestTrue("Complete Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromComposite(1))->WasCompleteActionTaskCalledNumTimes(1));
               TestTrue("Complete Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromComposite(2))->WasCompleteActionTaskCalledNumTimes(1));
               TestTrue("Complete Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromComposite(3))->WasCompleteActionTaskCalledNumTimes(1));
               TestTrue("Cancel Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromComposite(4))->WasCancelActionTaskCalledNumTimes(1));
            });

         It("should call the complete method of the composites when all tasks have completed, and honoring configuration of ignoring entries that do not need to be completed", [this]()
            {
               uint32 CompletionCalls = 0U;
               ActionTaskUnderTest->OnActionTaskComplete().AddLambda([&CompletionCalls](URpaiComposerActionTaskBase*, AAIController*, URpaiState*) -> void { ++CompletionCalls; });

               for (auto Idx = 0; Idx < 2; ++Idx)
               {
                  ActionTaskUnderTest->UpdateActionTask(GivenController, nullptr, 0.33f, GivenMemory);
               }

               TestEqual("Complete Called Once", CompletionCalls, 1U);
            });

         It("should call the cancel action task function on all entreis.", [this]()
            {
               uint32 CancelCalls = 0U;
               ActionTaskUnderTest->OnActionTaskCancelled().AddLambda([&CancelCalls](URpaiComposerActionTaskBase*, AAIController*, URpaiState*, bool) -> void { ++CancelCalls; });
               ActionTaskUnderTest->CancelActionTask(GivenController, nullptr, GivenMemory);

               TestTrue("Cancel Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromComposite(0))->WasCancelActionTaskCalledNumTimes(1));
               TestTrue("Cancel Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromComposite(1))->WasCancelActionTaskCalledNumTimes(1));
               TestTrue("Cancel Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromComposite(2))->WasCancelActionTaskCalledNumTimes(1));
               TestTrue("Cancel Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromComposite(3))->WasCancelActionTaskCalledNumTimes(1));
               TestTrue("Cancel Action Task", CastChecked<UTestComposerActionTask>(ActionTaskUnderTest->ViewTaskFromComposite(4))->WasCancelActionTaskCalledNumTimes(1));

               TestEqual("Cancel Called Once", CancelCalls, 1U);
            });

         AfterEach([this]()
            {
               GivenController->Destroy();
               ActionTaskUnderTest->ConditionalBeginDestroy();
            });
      });
}