#include "Misc/AutomationTest.h"
#include "ReasonablePlanningAITestTypes.h"
#include "Core/ReasonablePlanningResourceCollection.h"

BEGIN_DEFINE_SPEC(ReasonablePlanningResourceCollectionSpec, "ReasonablePlanningAI.ResourceCollection", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
	UReasonablePlanningResourceCollection* ClassUnderTest;
	UReasonablePlanningResourceCollection* LockingObject;
END_DEFINE_SPEC(ReasonablePlanningResourceCollectionSpec)
void ReasonablePlanningResourceCollectionSpec::Define()
{
	BeforeEach([this]()
		{
			ClassUnderTest = NewObject<UReasonablePlanningResourceCollection>();
			LockingObject = NewObject<UReasonablePlanningResourceCollection>();
		});

	Describe("Unlocked State", [this]()
		{
			It("Should be free", [this]()
				{
					TestTrue("IsResourceFree", ClassUnderTest->IsResourceFree("RPAI.Test.LockedResourceA"));
				});

			It("Should not allow unlocking", [this]()
				{
					TestFalse("UnlockResource", ClassUnderTest->UnlockResource(LockingObject, "RPAI.Test.LockedResourceA"));
				});

			It("Should not allow unlocking from a different lock object", [this]()
				{
					TestFalse("UnlockResource", ClassUnderTest->UnlockResource(ClassUnderTest, "RPAI.Test.LockedResourceA"));
				});

			It("Should indicate locks on heirarchy", [this]()
				{
					TestFalse("DoesHeirarchyHaveAnyLocks - RPAI", ClassUnderTest->DoesHeirarchyHaveAnyLocks("RPAI"));
					TestFalse("DoesHeirarchyHaveAnyLocks - RPAI.Test", ClassUnderTest->DoesHeirarchyHaveAnyLocks("RPAI.Test"));
					TestFalse("DoesHeirarchyHaveAnyLocks - RPAI.Test.LockedResourceA", ClassUnderTest->DoesHeirarchyHaveAnyLocks("RPAI.Test.LockedResourceA"));
					TestFalse("DoesHeirarchyHaveAnyLocks - Test", ClassUnderTest->DoesHeirarchyHaveAnyLocks("Test"));
					TestFalse("DoesHeirarchyHaveAnyLocks - Test.LockedResourceA", ClassUnderTest->DoesHeirarchyHaveAnyLocks("Test.LockedResourceA"));
					TestFalse("DoesHeirarchyHaveAnyLocks - LockedResource2", ClassUnderTest->DoesHeirarchyHaveAnyLocks("LockedResourceA"));
				});
		});

	Describe("Locking", [this]()
		{
			It("Should Allow Locking", [this]()
				{
					TestTrue("LockResource", ClassUnderTest->LockResource(LockingObject, "RPAI.Test.LockedResource2"));
				});
		});

	Describe("Locked State", [this]()
		{
			BeforeEach([this]()
				{
					ClassUnderTest->LockResource(LockingObject, "RPAI.Test.LockedResource2");
				});

			It("Should be locked", [this]()
				{
					TestTrue("IsResourceLocked", ClassUnderTest->IsResourceLocked("RPAI.Test.LockedResource2"));
				});

			It("Should not be free", [this]()
				{
					TestFalse("IsResourceFree", ClassUnderTest->IsResourceFree("RPAI.Test.LockedResource2"));
				});

			It("Should not be re-locked", [this]()
				{
					TestFalse("LockResource", ClassUnderTest->LockResource(LockingObject, "RPAI.Test.LockedResource2"));
				});

			It("Should not allow another object to lock the resource", [this]()
				{
					TestFalse("LockResource", ClassUnderTest->LockResource(ClassUnderTest, "RPAI.Test.LockedResource2"));
				});

			It("Should allow unlocking", [this]()
				{
					TestTrue("UnlockResource", ClassUnderTest->UnlockResource(LockingObject, "RPAI.Test.LockedResource2"));
				});

			It("Should not allow unlocking from a different lock object", [this]()
				{
					TestFalse("UnlockResource", ClassUnderTest->UnlockResource(ClassUnderTest, "RPAI.Test.LockedResource2"));
				});

			It("Should indicate locks on heirarchy", [this]()
				{
					TestTrue("DoesHeirarchyHaveAnyLocks - RPAI", ClassUnderTest->DoesHeirarchyHaveAnyLocks("RPAI"));
					TestTrue("DoesHeirarchyHaveAnyLocks - RPAI.Test", ClassUnderTest->DoesHeirarchyHaveAnyLocks("RPAI.Test"));
					TestTrue("DoesHeirarchyHaveAnyLocks - RPAI.Test.LockedResource2", ClassUnderTest->DoesHeirarchyHaveAnyLocks("RPAI.Test.LockedResource2"));
					TestFalse("DoesHeirarchyHaveAnyLocks - Test", ClassUnderTest->DoesHeirarchyHaveAnyLocks("Test"));
					TestFalse("DoesHeirarchyHaveAnyLocks - Test.LockedResource2", ClassUnderTest->DoesHeirarchyHaveAnyLocks("Test.LockedResource2"));
					TestFalse("DoesHeirarchyHaveAnyLocks - LockedResource2", ClassUnderTest->DoesHeirarchyHaveAnyLocks("LockedResource2"));
				});

			AfterEach([this]()
				{
					ClassUnderTest->UnlockResource(LockingObject, "RPAI.Test.LockedResource2");
				});
		});

	AfterEach([this]()
		{
			ClassUnderTest->ConditionalBeginDestroy();
			LockingObject->ConditionalBeginDestroy();
		});
}
