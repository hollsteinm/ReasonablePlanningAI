// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "ReasonablePlanningAITestTypes.h"
#include "Core/RpaiResourceCollection.h"

BEGIN_DEFINE_SPEC(ReasonablePlanningResourceCollectionSpec, "ReasonablePlanningAI.ResourceCollection", EAutomationTestFlags::ProductFilter | EAutomationTestFlags_ApplicationContextMask)
   URpaiResourceCollection* ClassUnderTest;
   URpaiResourceCollection* LockingObject;
END_DEFINE_SPEC(ReasonablePlanningResourceCollectionSpec)
void ReasonablePlanningResourceCollectionSpec::Define()
{
   BeforeEach([this]()
      {
         ClassUnderTest = NewObject<URpaiResourceCollection>();
         LockingObject = NewObject<URpaiResourceCollection>();
      });

   Describe("Unlocked State", [this]()
      {
         It("Should be free", [this]()
            {
               TestTrue("IsResourceFree", ClassUnderTest->IsResourceFree("Rpai.Test.LockedResourceA"));
            });

         It("Should not allow unlocking", [this]()
            {
               TestFalse("UnlockResource", ClassUnderTest->UnlockResource(LockingObject, "Rpai.Test.LockedResourceA"));
            });

         It("Should not allow unlocking from a different lock object", [this]()
            {
               TestFalse("UnlockResource", ClassUnderTest->UnlockResource(ClassUnderTest, "Rpai.Test.LockedResourceA"));
            });

         It("Should indicate locks on heirarchy", [this]()
            {
               TestFalse("DoesHeirarchyHaveAnyLocks - Rpai", ClassUnderTest->DoesHeirarchyHaveAnyLocks("Rpai"));
               TestFalse("DoesHeirarchyHaveAnyLocks - Rpai.Test", ClassUnderTest->DoesHeirarchyHaveAnyLocks("Rpai.Test"));
               TestFalse("DoesHeirarchyHaveAnyLocks - Rpai.Test.LockedResourceA", ClassUnderTest->DoesHeirarchyHaveAnyLocks("Rpai.Test.LockedResourceA"));
               TestFalse("DoesHeirarchyHaveAnyLocks - Test", ClassUnderTest->DoesHeirarchyHaveAnyLocks("Test"));
               TestFalse("DoesHeirarchyHaveAnyLocks - Test.LockedResourceA", ClassUnderTest->DoesHeirarchyHaveAnyLocks("Test.LockedResourceA"));
               TestFalse("DoesHeirarchyHaveAnyLocks - LockedResource2", ClassUnderTest->DoesHeirarchyHaveAnyLocks("LockedResourceA"));
            });
      });

   Describe("Locking", [this]()
      {
         It("Should Allow Locking", [this]()
            {
               TestTrue("LockResource", ClassUnderTest->LockResource(LockingObject, "Rpai.Test.LockedResource2"));
            });
      });

   Describe("Locked State", [this]()
      {
         BeforeEach([this]()
            {
               ClassUnderTest->LockResource(LockingObject, "Rpai.Test.LockedResource2");
            });

         It("Should be locked", [this]()
            {
               TestTrue("IsResourceLocked", ClassUnderTest->IsResourceLocked("Rpai.Test.LockedResource2"));
            });

         It("Should not be free", [this]()
            {
               TestFalse("IsResourceFree", ClassUnderTest->IsResourceFree("Rpai.Test.LockedResource2"));
            });

         It("Should not be re-locked", [this]()
            {
               TestFalse("LockResource", ClassUnderTest->LockResource(LockingObject, "Rpai.Test.LockedResource2"));
            });

         It("Should not allow another object to lock the resource", [this]()
            {
               TestFalse("LockResource", ClassUnderTest->LockResource(ClassUnderTest, "Rpai.Test.LockedResource2"));
            });

         It("Should allow unlocking", [this]()
            {
               TestTrue("UnlockResource", ClassUnderTest->UnlockResource(LockingObject, "Rpai.Test.LockedResource2"));
            });

         It("Should not allow unlocking from a different lock object", [this]()
            {
               TestFalse("UnlockResource", ClassUnderTest->UnlockResource(ClassUnderTest, "Rpai.Test.LockedResource2"));
            });

         It("Should indicate locks on heirarchy", [this]()
            {
               TestTrue("DoesHeirarchyHaveAnyLocks - Rpai", ClassUnderTest->DoesHeirarchyHaveAnyLocks("Rpai"));
               TestTrue("DoesHeirarchyHaveAnyLocks - Rpai.Test", ClassUnderTest->DoesHeirarchyHaveAnyLocks("Rpai.Test"));
               TestTrue("DoesHeirarchyHaveAnyLocks - Rpai.Test.LockedResource2", ClassUnderTest->DoesHeirarchyHaveAnyLocks("Rpai.Test.LockedResource2"));
               TestFalse("DoesHeirarchyHaveAnyLocks - Test", ClassUnderTest->DoesHeirarchyHaveAnyLocks("Test"));
               TestFalse("DoesHeirarchyHaveAnyLocks - Test.LockedResource2", ClassUnderTest->DoesHeirarchyHaveAnyLocks("Test.LockedResource2"));
               TestFalse("DoesHeirarchyHaveAnyLocks - LockedResource2", ClassUnderTest->DoesHeirarchyHaveAnyLocks("LockedResource2"));
            });

         AfterEach([this]()
            {
               ClassUnderTest->UnlockResource(LockingObject, "Rpai.Test.LockedResource2");
            });
      });

   AfterEach([this]()
      {
         ClassUnderTest->ConditionalBeginDestroy();
         LockingObject->ConditionalBeginDestroy();
      });
}
