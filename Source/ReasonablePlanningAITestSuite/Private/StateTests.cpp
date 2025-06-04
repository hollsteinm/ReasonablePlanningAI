// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "ReasonablePlanningAITestTypes.h"
#include "States/RpaiState_Map.h"
#include "States/RpaiState_Reflection.h"
#include "ReasonablePlanningAITestTypes.h"

BEGIN_DEFINE_SPEC(ReasonablePlanningStateMapSpec, "ReasonablePlanningAI.StateMap", EAutomationTestFlags::ProductFilter | EAutomationTestFlags_ApplicationContextMask)
   URpaiState_Map* ClassUnderTest;
END_DEFINE_SPEC(ReasonablePlanningStateMapSpec)
void ReasonablePlanningStateMapSpec::Define()
{
   BeforeEach([this]()
      {
         ClassUnderTest = NewObject<URpaiState_Map>();
         ClassUnderTest->SetAsDynamic(true);
         ClassUnderTest->SetBool(UTestPlanningState::NAME_TheBoolValue, false);
         ClassUnderTest->SetClassValue(UTestPlanningState::NAME_TheClassValue, URpaiState_Map::StaticClass());
         ClassUnderTest->SetFloat(UTestPlanningState::NAME_TheFloatValue, 0.f);
         ClassUnderTest->SetInt(UTestPlanningState::NAME_TheIntValue, 0);
         ClassUnderTest->SetNameValue(UTestPlanningState::NAME_TheNameValue, NAME_None);
         ClassUnderTest->SetObject(UTestPlanningState::NAME_TheObjectValue, NewObject<URpaiState_Map>(ClassUnderTest));
         ClassUnderTest->SetRotator(UTestPlanningState::NAME_TheRotatorValue, FRotator::ZeroRotator);
         ClassUnderTest->SetString(UTestPlanningState::NAME_TheStringValue, FString("Hello World"));
         ClassUnderTest->SetVector(UTestPlanningState::NAME_TheVectorValue, FVector::ZeroVector);
         ClassUnderTest->SetAsDynamic(false);
      });

   Describe("Equality", [this]()
      {
         It("should be equal", [this]()
            {
               URpaiState* Copy = NewObject<URpaiState_Map>();
               ClassUnderTest->CopyStateForPredictionTo(Copy);
               TestTrue("URpaiState::IsEqualTo", ClassUnderTest->IsEqualTo(Copy));
               TestTrue("URpaiState::IsEqualTo", ClassUnderTest->IsEqualTo(ClassUnderTest));
            });

         It("should not be equal", [this]()
            {
               URpaiState* Copy = NewObject<URpaiState_Map>();
               TestFalse("URpaiState::IsEqualTo", ClassUnderTest->IsEqualTo(Copy));
            });
      });

   Describe("Getting Values", [this]()
      {
         It("should get the defined boolean value", [this]()
            {
               bool Value = true;
               TestTrue("HasBool", ClassUnderTest->HasBool(UTestPlanningState::NAME_TheBoolValue));
               TestTrue("HasValueWithName", ClassUnderTest->HasValueWithName(UTestPlanningState::NAME_TheBoolValue));
               TestTrue("GetBool", ClassUnderTest->GetBool(UTestPlanningState::NAME_TheBoolValue, Value));
               TestFalse("The Out Value of Get Bool", Value);
            });

         It("should get the defined class value", [this]()
            {
               UClass* Clazz = nullptr;
               TestTrue("HasClass", ClassUnderTest->HasClass(UTestPlanningState::NAME_TheClassValue));
               TestTrue("HasValueWithName", ClassUnderTest->HasValueWithName(UTestPlanningState::NAME_TheClassValue));
               TestTrue("GetClassValue", ClassUnderTest->GetClassValue(UTestPlanningState::NAME_TheClassValue, Clazz));
               TestEqual("Class is equal", Clazz, URpaiState_Map::StaticClass());
            });

         It("should get the defined float value", [this]()
            {
               float Value = 1000.f;
               TestTrue("HasFloat", ClassUnderTest->HasFloat(UTestPlanningState::NAME_TheFloatValue));
               TestTrue("HasValueWithName", ClassUnderTest->HasValueWithName(UTestPlanningState::NAME_TheFloatValue));
               TestTrue("GetFloat", ClassUnderTest->GetFloat(UTestPlanningState::NAME_TheFloatValue, Value));
               TestEqual("Float is equal", Value, 0.f);
            });

         It("should get the defined integer value", [this]()
            {
               int32 Value = 32;
               TestTrue("HasInt", ClassUnderTest->HasInt(UTestPlanningState::NAME_TheIntValue));
               TestTrue("HasValueWithName", ClassUnderTest->HasValueWithName(UTestPlanningState::NAME_TheIntValue));
               TestTrue("GetInt", ClassUnderTest->GetInt(UTestPlanningState::NAME_TheIntValue, Value));
               TestEqual("Int is equal", Value, 0);
            });

         It("should get the defined Name value", [this]()
            {
               FName Value = "TEST_NAME";
               TestTrue("HasName", ClassUnderTest->HasName(UTestPlanningState::NAME_TheNameValue));
               TestTrue("HasValueWithName", ClassUnderTest->HasValueWithName(UTestPlanningState::NAME_TheNameValue));
               TestTrue("GetNameValue", ClassUnderTest->GetNameValue(UTestPlanningState::NAME_TheNameValue, Value));
               TestTrue("Name is NONE", Value.IsNone());
            });

         It("should get the defined object value", [this]()
            {
               UObject* Value = nullptr;
               TestTrue("HasObject", ClassUnderTest->HasObject(UTestPlanningState::NAME_TheObjectValue));
               TestTrue("HasValueWithName", ClassUnderTest->HasValueWithName(UTestPlanningState::NAME_TheObjectValue));
               TestTrue("GetObject", ClassUnderTest->GetObject(UTestPlanningState::NAME_TheObjectValue, Value));
               TestNotEqual("Pointer is valid", Value, static_cast<UObject*>(nullptr));
               TestEqual("Is object", Value->GetOuter(), static_cast<UObject*>(ClassUnderTest));
            });

         It("should get the defined rotator value", [this]()
            {
               FRotator Value(3.f, 1.f, 2.f);
               TestTrue("HasRotator", ClassUnderTest->HasRotator(UTestPlanningState::NAME_TheRotatorValue));
               TestTrue("HasValueWithName", ClassUnderTest->HasValueWithName(UTestPlanningState::NAME_TheRotatorValue));
               TestTrue("GetRotator", ClassUnderTest->GetRotator(UTestPlanningState::NAME_TheRotatorValue, Value));
               TestEqual("The value is from state", Value, FRotator::ZeroRotator);
            });

         It("should get the defined string value", [this]()
            {
               FString Value = "Test";
               TestTrue("HasString", ClassUnderTest->HasString(UTestPlanningState::NAME_TheStringValue));
               TestTrue("HasValueWithName", ClassUnderTest->HasValueWithName(UTestPlanningState::NAME_TheStringValue));
               TestTrue("GetString", ClassUnderTest->GetString(UTestPlanningState::NAME_TheStringValue, Value));
               TestEqual("The value is from state", Value, "Hello World");
            });

         It("should get the defined vector value", [this]()
            {
               FVector Value(3.f, 1.f, 2.f);
               TestTrue("HasVector", ClassUnderTest->HasVector(UTestPlanningState::NAME_TheVectorValue));
               TestTrue("HasValueWithName", ClassUnderTest->HasValueWithName(UTestPlanningState::NAME_TheVectorValue));
               TestTrue("GetVector", ClassUnderTest->GetVector(UTestPlanningState::NAME_TheVectorValue, Value));
               TestEqual("The value is from state", Value, FVector::ZeroVector);
            });
      });

   Describe("Dynamic State", [this]()
      {
         BeforeEach([this]()
            {
               ClassUnderTest->SetAsDynamic(true);
            });

         It("should set and create a new bool value", [this]()
            {
               bool Value = true;
               const bool Expected = true;
               const FName Key = "ABoolValue";
               ClassUnderTest->SetBool(Key, Value);
               TestTrue("HasBool", ClassUnderTest->HasBool(Key));
               TestTrue("HasValueWithName", ClassUnderTest->HasValueWithName(Key));
               TestTrue("GetBool", ClassUnderTest->GetBool(Key, Value));
               TestEqual("The Out Value of Get Bool", Value, Expected);
            });

         It("should set and create a new class value", [this]()
            {
               UClass* Clazz = UTestPlanningState::StaticClass();
               const FName Key = "AClassValue";
               ClassUnderTest->SetClassValue(Key, Clazz);
               TestTrue("HasClass", ClassUnderTest->HasClass(Key));
               TestTrue("HasValueWithName", ClassUnderTest->HasValueWithName(Key));
               TestTrue("GetClassValue", ClassUnderTest->GetClassValue(Key, Clazz));
               TestEqual("Class is equal", Clazz, UTestPlanningState::StaticClass());
            });

         It("should set and create a new float value", [this]()
            {
               float Value = 566.f;
               const float Expected = 566.f;
               const FName Key = "AFloatValue";
               ClassUnderTest->SetFloat(Key, Value);
               TestTrue("HasFloat", ClassUnderTest->HasFloat(Key));
               TestTrue("HasValueWithName", ClassUnderTest->HasValueWithName(Key));
               TestTrue("GetFloat", ClassUnderTest->GetFloat(Key, Value));
               TestEqual("Float is equal", Value, Expected);
            });

         It("should set and create a new int value", [this]()
            {
               int32 Value = 32;
               const int32 Expected = Value;
               const FName Key = "AIntValue";
               ClassUnderTest->SetInt(Key, Value);
               TestTrue("HasInt", ClassUnderTest->HasInt(Key));
               TestTrue("HasValueWithName", ClassUnderTest->HasValueWithName(Key));
               TestTrue("GetInt", ClassUnderTest->GetInt(Key, Value));
               TestEqual("Int is equal", Value, Expected);
            });

         It("should set and create a new name value", [this]()
            {
               FName Value = "TEST_NAME";
               const FName Expected = "TEST_NAME";
               const FName Key = "ANameValue";
               ClassUnderTest->SetNameValue(Key, Value);
               TestTrue("HasName", ClassUnderTest->HasName(Key));
               TestTrue("HasValueWithName", ClassUnderTest->HasValueWithName(Key));
               TestTrue("GetNameValue", ClassUnderTest->GetNameValue(Key, Value));
               TestEqual("Name is set", Value, Expected);
            });

         It("should set and create a new object value", [this]()
            {
               UObject* Value = NewObject<URpaiState_Map>(ClassUnderTest);
               UObject* Expected = Value;
               const FName Key = "AnObjectValue";
               ClassUnderTest->SetObject(Key, Value);
               TestTrue("HasObject", ClassUnderTest->HasObject(Key));
               TestTrue("HasValueWithName", ClassUnderTest->HasValueWithName(Key));
               TestTrue("GetObject", ClassUnderTest->GetObject(Key, Value));
               TestEqual("Is object", Value, Expected);
            });

         It("should set and create a new rotator value", [this]()
            {
               FRotator Value(3.f, 1.f, 2.f);
               const FRotator Expected = Value;
               const FName Key = "TestRotatorValue";
               ClassUnderTest->SetRotator(Key, Value);
               TestTrue("HasRotator", ClassUnderTest->HasRotator(Key));
               TestTrue("HasValueWithName", ClassUnderTest->HasValueWithName(Key));
               TestTrue("GetRotator", ClassUnderTest->GetRotator(Key, Value));
               TestEqual("The value is from state", Value, Expected);
            });

         It("should set and create a new string value", [this]()
            {
               FString Value = "Test";
               const FString Expected = Value;
               const FName Key = "TestStringValue";
               ClassUnderTest->SetString(Key, Value);
               TestTrue("HasString", ClassUnderTest->HasString(Key));
               TestTrue("HasValueWithName", ClassUnderTest->HasValueWithName(Key));
               TestTrue("GetString", ClassUnderTest->GetString(Key, Value));
               TestEqual("The value is from state", Value, Expected);
            });

         It("should set and create a new vector value", [this]()
            {
               FVector Value(3.f, 1.f, 2.f);
               const FVector Expected = Value;
               const FName Key = "TestVectorValue";
               ClassUnderTest->SetVector(Key, Value);
               TestTrue("HasVector", ClassUnderTest->HasVector(Key));
               TestTrue("HasValueWithName", ClassUnderTest->HasValueWithName(Key));
               TestTrue("GetVector", ClassUnderTest->GetVector(Key, Value));
               TestEqual("The value is from state", Value, Expected);
            });
      });

   Describe("Immutable State Names", [this]()
      {
         BeforeEach([this]()
            {
               ClassUnderTest->SetAsDynamic(false);
            });

         It("should not set and create a new bool value", [this]()
            {
               bool Value = true;
               const FName Key = "ABoolValue2";
               ClassUnderTest->SetBool(Key, Value);
               TestFalse("HasBool", ClassUnderTest->HasBool(Key));
               TestFalse("HasValueWithName", ClassUnderTest->HasValueWithName(Key));
               TestFalse("GetBool", ClassUnderTest->GetBool(Key, Value));
            });

         It("should not set and create a new class value", [this]()
            {
               UClass* Clazz = UTestPlanningState::StaticClass();
               const FName Key = "AClassValue2";
               ClassUnderTest->SetClassValue(Key, Clazz);
               TestFalse("HasClass", ClassUnderTest->HasClass(Key));
               TestFalse("HasValueWithName", ClassUnderTest->HasValueWithName(Key));
               TestFalse("GetClassValue", ClassUnderTest->GetClassValue(Key, Clazz));
            });

         It("should not set and create a new float value", [this]()
            {
               float Value = 566.f;
               const FName Key = "AFloatValue2";
               ClassUnderTest->SetFloat(Key, Value);
               TestFalse("HasFloat", ClassUnderTest->HasFloat(Key));
               TestFalse("HasValueWithName", ClassUnderTest->HasValueWithName(Key));
               TestFalse("GetFloat", ClassUnderTest->GetFloat(Key, Value));
            });

         It("should not set and create a new int value", [this]()
            {
               int32 Value = 32;
               const FName Key = "AIntValue2";
               ClassUnderTest->SetInt(Key, Value);
               TestFalse("HasInt", ClassUnderTest->HasInt(Key));
               TestFalse("HasValueWithName", ClassUnderTest->HasValueWithName(Key));
               TestFalse("GetInt", ClassUnderTest->GetInt(Key, Value));
            });

         It("should not set and create a new name value", [this]()
            {
               FName Value = "TEST_NAME";
               const FName Key = "ANameValue2";
               ClassUnderTest->SetNameValue(Key, Value);
               TestFalse("HasName", ClassUnderTest->HasName(Key));
               TestFalse("HasValueWithName", ClassUnderTest->HasValueWithName(Key));
               TestFalse("GetNameValue", ClassUnderTest->GetNameValue(Key, Value));
            });

         It("should not set and create a new object value", [this]()
            {
               UObject* Value = NewObject<URpaiState_Map>(ClassUnderTest);
               const FName Key = "AnObjectValue2";
               ClassUnderTest->SetObject(Key, Value);
               TestFalse("HasObject", ClassUnderTest->HasObject(Key));
               TestFalse("HasValueWithName", ClassUnderTest->HasValueWithName(Key));
               TestFalse("GetObject", ClassUnderTest->GetObject(Key, Value));
            });

         It("should not set and create a new rotator value", [this]()
            {
               FRotator Value(3.f, 1.f, 2.f);
               const FName Key = "TestRotatorValue2";
               ClassUnderTest->SetRotator(Key, Value);
               TestFalse("HasRotator", ClassUnderTest->HasRotator(Key));
               TestFalse("HasValueWithName", ClassUnderTest->HasValueWithName(Key));
               TestFalse("GetRotator", ClassUnderTest->GetRotator(Key, Value));
            });

         It("should not set and create a new string value", [this]()
            {
               FString Value = "Test";
               const FName Key = "TestStringValue2";
               ClassUnderTest->SetString(Key, Value);
               TestFalse("HasString", ClassUnderTest->HasString(Key));
               TestFalse("HasValueWithName", ClassUnderTest->HasValueWithName(Key));
               TestFalse("GetString", ClassUnderTest->GetString(Key, Value));
            });

         It("should not set and create a new vector value", [this]()
            {
               FVector Value(3.f, 1.f, 2.f);
               const FName Key = "TestVectorValue2";
               ClassUnderTest->SetVector(Key, Value);
               TestFalse("HasVector", ClassUnderTest->HasVector(Key));
               TestFalse("HasValueWithName", ClassUnderTest->HasValueWithName(Key));
               TestFalse("GetVector", ClassUnderTest->GetVector(Key, Value));
            });
      });

   AfterEach([this]()
      {
         ClassUnderTest->ConditionalBeginDestroy();
      });

   Describe("Resource Locking", [this]()
      {
         Describe("Unlocked State", [this]()
            {
               It("Should be free", [this]()
                  {
                     TestTrue("IsResourceFree", ClassUnderTest->IsResourceFree("Rpai.Test.LockedResourceA"));
                  });

               It("Should not allow unlocking", [this]()
                  {
                     TestFalse("UnlockResource", ClassUnderTest->UnlockResource("Rpai.Test.LockedResourceA"));
                  });

               It("Should not allow unlocking from a different lock object", [this]()
                  {
                     URpaiState_Map* Temp = NewObject<URpaiState_Map>(ClassUnderTest);
                     TestFalse("UnlockResource", ClassUnderTest->UnlockResource("Rpai.Test.LockedResourceA"));
                     Temp->ConditionalBeginDestroy();
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
                     TestTrue("LockResource", ClassUnderTest->LockResource("Rpai.Test.LockedResource2"));
                  });
            });

         Describe("Locked State", [this]()
            {
               BeforeEach([this]()
                  {
                     ClassUnderTest->LockResource("Rpai.Test.LockedResource2");
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
                     TestFalse("LockResource", ClassUnderTest->LockResource("Rpai.Test.LockedResource2"));
                  });

               It("Should not allow another object to lock the resource", [this]()
                  {
                     URpaiState_Map* Temp = NewObject<URpaiState_Map>(ClassUnderTest);
                     TestFalse("LockResource", ClassUnderTest->LockResource("Rpai.Test.LockedResource2"));
                     Temp->ConditionalBeginDestroy();
                  });

               It("Should allow unlocking", [this]()
                  {
                     TestTrue("UnlockResource", ClassUnderTest->UnlockResource("Rpai.Test.LockedResource2"));
                  });

               It("Should not allow unlocking from a different lock object", [this]()
                  {
                     URpaiState_Map* Temp = NewObject<URpaiState_Map>(ClassUnderTest);
                     TestFalse("UnlockResource", ClassUnderTest->UnlockResource("Rpai.Test.LockedResource2", Temp));
                     Temp->ConditionalBeginDestroy();
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
                     ClassUnderTest->UnlockResource("Rpai.Test.LockedResource2");
                  });
            });
      });
}

BEGIN_DEFINE_SPEC(ReasonablePlanningStateReflectionSpec, "ReasonablePlanningAI.StateReflection", EAutomationTestFlags::ProductFilter | EAutomationTestFlags_ApplicationContextMask)
   URpaiState_Reflection* ClassUnderTest;
END_DEFINE_SPEC(ReasonablePlanningStateReflectionSpec)
void ReasonablePlanningStateReflectionSpec::Define()
{
   BeforeEach([this]()
      {
         ClassUnderTest = NewObject<UTestPlanningState>();
         ClassUnderTest->SetBool(UTestPlanningState::NAME_TheBoolValue, false);
         ClassUnderTest->SetClassValue(UTestPlanningState::NAME_TheClassValue, URpaiState_Map::StaticClass());
         ClassUnderTest->SetFloat(UTestPlanningState::NAME_TheFloatValue, 0.f);
         ClassUnderTest->SetInt(UTestPlanningState::NAME_TheIntValue, 0);
         ClassUnderTest->SetNameValue(UTestPlanningState::NAME_TheNameValue, NAME_None);
         ClassUnderTest->SetObject(UTestPlanningState::NAME_TheObjectValue, NewObject<URpaiState_Map>(ClassUnderTest));
         ClassUnderTest->SetRotator(UTestPlanningState::NAME_TheRotatorValue, FRotator::ZeroRotator);
         ClassUnderTest->SetString(UTestPlanningState::NAME_TheStringValue, FString("Hello World"));
         ClassUnderTest->SetVector(UTestPlanningState::NAME_TheVectorValue, FVector::ZeroVector);
      });

   Describe("Equality", [this]()
      {
         It("should be equal", [this]()
            {
               URpaiState* Copy = NewObject<UTestPlanningState>();
               ClassUnderTest->CopyStateForPredictionTo(Copy);
               TestTrue("URpaiState::IsEqualTo", ClassUnderTest->IsEqualTo(Copy));
               TestTrue("URpaiState::IsEqualTo", ClassUnderTest->IsEqualTo(ClassUnderTest));
            });

         It("should not be equal", [this]()
            {
               URpaiState* Copy = NewObject<UTestPlanningState>();
               bool bFlip = false;
               ClassUnderTest->GetBool(UTestPlanningState::NAME_TheBoolValue, bFlip);
               Copy->SetBool(UTestPlanningState::NAME_TheBoolValue, !bFlip);
               TestFalse("URpaiState::IsEqualTo", ClassUnderTest->IsEqualTo(Copy));
            });
      });

   Describe("Getting Values", [this]()
      {
         It("should get the defined boolean value", [this]()
            {
               bool Value = true;
               TestTrue("HasBool", ClassUnderTest->HasBool(UTestPlanningState::NAME_TheBoolValue));
               TestTrue("HasValueWithName", ClassUnderTest->HasValueWithName(UTestPlanningState::NAME_TheBoolValue));
               TestTrue("GetBool", ClassUnderTest->GetBool(UTestPlanningState::NAME_TheBoolValue, Value));
               TestFalse("The Out Value of Get Bool", Value);
            });

         It("should get the defined class value", [this]()
            {
               UClass* Clazz = nullptr;
               TestTrue("HasClass", ClassUnderTest->HasClass(UTestPlanningState::NAME_TheClassValue));
               TestTrue("HasValueWithName", ClassUnderTest->HasValueWithName(UTestPlanningState::NAME_TheClassValue));
               TestTrue("GetClassValue", ClassUnderTest->GetClassValue(UTestPlanningState::NAME_TheClassValue, Clazz));
               TestEqual("Class is equal", Clazz, URpaiState_Map::StaticClass());
            });

         It("should get the defined float value", [this]()
            {
               float Value = 1000.f;
               TestTrue("HasFloat", ClassUnderTest->HasFloat(UTestPlanningState::NAME_TheFloatValue));
               TestTrue("HasValueWithName", ClassUnderTest->HasValueWithName(UTestPlanningState::NAME_TheFloatValue));
               TestTrue("GetFloat", ClassUnderTest->GetFloat(UTestPlanningState::NAME_TheFloatValue, Value));
               TestEqual("Float is equal", Value, 0.f);
            });

         It("should get the defined integer value", [this]()
            {
               int32 Value = 32;
               TestTrue("HasInt", ClassUnderTest->HasInt(UTestPlanningState::NAME_TheIntValue));
               TestTrue("HasValueWithName", ClassUnderTest->HasValueWithName(UTestPlanningState::NAME_TheIntValue));
               TestTrue("GetInt", ClassUnderTest->GetInt(UTestPlanningState::NAME_TheIntValue, Value));
               TestEqual("Int is equal", Value, 0);
            });

         It("should get the defined Name value", [this]()
            {
               FName Value = "TEST_NAME";
               TestTrue("HasName", ClassUnderTest->HasName(UTestPlanningState::NAME_TheNameValue));
               TestTrue("HasValueWithName", ClassUnderTest->HasValueWithName(UTestPlanningState::NAME_TheNameValue));
               TestTrue("GetNameValue", ClassUnderTest->GetNameValue(UTestPlanningState::NAME_TheNameValue, Value));
               TestTrue("Name is NONE", Value.IsNone());
            });

         It("should get the defined object value", [this]()
            {
               UObject* Value = nullptr;
               TestTrue("HasObject", ClassUnderTest->HasObject(UTestPlanningState::NAME_TheObjectValue));
               TestTrue("HasValueWithName", ClassUnderTest->HasValueWithName(UTestPlanningState::NAME_TheObjectValue));
               TestTrue("GetObject", ClassUnderTest->GetObject(UTestPlanningState::NAME_TheObjectValue, Value));
               TestNotEqual("Pointer is valid", Value, static_cast<UObject*>(nullptr));
               TestEqual("Is object", Value->GetOuter(), static_cast<UObject*>(ClassUnderTest));
            });

         It("should get the defined rotator value", [this]()
            {
               FRotator Value(3.f, 1.f, 2.f);
               TestTrue("HasRotator", ClassUnderTest->HasRotator(UTestPlanningState::NAME_TheRotatorValue));
               TestTrue("HasValueWithName", ClassUnderTest->HasValueWithName(UTestPlanningState::NAME_TheRotatorValue));
               TestTrue("GetRotator", ClassUnderTest->GetRotator(UTestPlanningState::NAME_TheRotatorValue, Value));
               TestEqual("The value is from state", Value, FRotator::ZeroRotator);
            });

         It("should get the defined string value", [this]()
            {
               FString Value = "Test";
               TestTrue("HasString", ClassUnderTest->HasString(UTestPlanningState::NAME_TheStringValue));
               TestTrue("HasValueWithName", ClassUnderTest->HasValueWithName(UTestPlanningState::NAME_TheStringValue));
               TestTrue("GetString", ClassUnderTest->GetString(UTestPlanningState::NAME_TheStringValue, Value));
               TestEqual("The value is from state", Value, "Hello World");
            });

         It("should get the defined vector value", [this]()
            {
               FVector Value(3.f, 1.f, 2.f);
               TestTrue("HasVector", ClassUnderTest->HasVector(UTestPlanningState::NAME_TheVectorValue));
               TestTrue("HasValueWithName", ClassUnderTest->HasValueWithName(UTestPlanningState::NAME_TheVectorValue));
               TestTrue("GetVector", ClassUnderTest->GetVector(UTestPlanningState::NAME_TheVectorValue, Value));
               TestEqual("The value is from state", Value, FVector::ZeroVector);
            });
      });

   AfterEach([this]()
      {
         ClassUnderTest->ConditionalBeginDestroy();
      });

   Describe("Resource Locking", [this]()
      {
         Describe("Unlocked State", [this]()
            {
               It("Should be free", [this]()
                  {
                     TestTrue("IsResourceFree", ClassUnderTest->IsResourceFree("Rpai.Test.LockedResourceA"));
                  });

               It("Should not allow unlocking", [this]()
                  {
                     TestFalse("UnlockResource", ClassUnderTest->UnlockResource("Rpai.Test.LockedResourceA"));
                  });

               It("Should not allow unlocking from a different lock object", [this]()
                  {
                     URpaiState_Reflection* Temp = NewObject<UTestPlanningState>(ClassUnderTest);
                     TestFalse("UnlockResource", ClassUnderTest->UnlockResource("Rpai.Test.LockedResourceA"));
                     Temp->ConditionalBeginDestroy();
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
                     TestTrue("LockResource", ClassUnderTest->LockResource("Rpai.Test.LockedResource2"));
                  });
            });

         Describe("Locked State", [this]()
            {
               BeforeEach([this]()
                  {
                     ClassUnderTest->LockResource("Rpai.Test.LockedResource2");
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
                     TestFalse("LockResource", ClassUnderTest->LockResource("Rpai.Test.LockedResource2"));
                  });

               It("Should not allow another object to lock the resource", [this]()
                  {
                     URpaiState_Reflection* Temp = NewObject<UTestPlanningState>(ClassUnderTest);
                     TestFalse("LockResource", ClassUnderTest->LockResource("Rpai.Test.LockedResource2"));
                     Temp->ConditionalBeginDestroy();
                  });

               It("Should allow unlocking", [this]()
                  {
                     TestTrue("UnlockResource", ClassUnderTest->UnlockResource("Rpai.Test.LockedResource2"));
                  });

               It("Should not allow unlocking from a different lock object", [this]()
                  {
                     URpaiState_Reflection* Temp = NewObject<UTestPlanningState>(ClassUnderTest);
                     TestFalse("UnlockResource", ClassUnderTest->UnlockResource("Rpai.Test.LockedResource2", Temp));
                     Temp->ConditionalBeginDestroy();
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
                     ClassUnderTest->UnlockResource("Rpai.Test.LockedResource2");
                  });
            });
      });
}

BEGIN_DEFINE_SPEC(ReasonablePlanningStateBindingSpec, "ReasonablePlanningAI.StateBinding", EAutomationTestFlags::ProductFilter | EAutomationTestFlags_ApplicationContextMask)
   URpaiState* SourceState;
END_DEFINE_SPEC(ReasonablePlanningStateBindingSpec)
void ReasonablePlanningStateBindingSpec::Define()
{
   BeforeEach([this]()
      {
         SourceState = NewObject<UTestPlanningState>();
      });

   Describe("Behavior", [this]()
      {
         It("should register handles", [this]()
            {
               FRpaiStateBinding Binding;
               FStateBindingHandle Handle = Binding.AddBinding(SourceState->GetClass(), TEXT("TheFloatValue"), FComplexTestStruct::StaticStruct(), TEXT("Value"));
               TestTrue("Handle::IsValid", Handle.IsValid());
               TestTrue("Handle::SourceType::IsValid", Handle.StateSourceTypeHandle.IsValid());
               TestTrue("Handle::SourceProperty::IsValid", Handle.StateSourcePropertyHandle.IsValid());
               TestTrue("Handle::TargetType::IsValid", Handle.StateTargetTypeHandle.IsValid());
               TestTrue("Handle::TargetProperty::IsValid", Handle.StateTargetPropertyHandle.IsValid());

               Handle = Binding.AddBinding(SourceState->GetClass(), TEXT("TheIntValue"), FComplexTestStruct::StaticStruct(), TEXT("Inner.Value"));
               TestTrue("Handle::IsValid", Handle.IsValid());
               TestTrue("Handle::SourceType::IsValid", Handle.StateSourceTypeHandle.IsValid());
               TestTrue("Handle::SourceProperty::IsValid", Handle.StateSourcePropertyHandle.IsValid());
               TestTrue("Handle::TargetType::IsValid", Handle.StateTargetTypeHandle.IsValid());
               TestTrue("Handle::TargetProperty::IsValid", Handle.StateTargetPropertyHandle.IsValid());
            });
        
            It("should copy values", [this]()
                {
                    FRpaiStateBinding Binding;
                    Binding.AddBinding(SourceState->GetClass(), TEXT("TheFloatValue"), FComplexTestStruct::StaticStruct(), TEXT("Value"));
                    Binding.AddBinding(SourceState->GetClass(), TEXT("TheIntValue"), FComplexTestStruct::StaticStruct(), TEXT("Inner.Value"));
            
                
                    SourceState->SetFloat("TheFloatValue", -56.098f);
                    SourceState->SetInt("TheIntValue", 9999999);
                
                    FRpaiMemory TestMemory;
                    FRpaiMemoryStruct TargetMemory(&TestMemory, FComplexTestStruct::StaticStruct());
                    FComplexTestStruct* TargetState = TargetMemory.Get<FComplexTestStruct>();
                    TargetState->Value = 101.f;
                    TargetState->Inner.Value = 4000;
                
                    Binding.Transfer(SourceState, TargetMemory);
                
                    TestEqual(TEXT("float value copied"), -56.098f, TargetState->Value);
                    TestEqual(TEXT("complexx inner value copied"), 9999999, TargetState->Inner.Value);
                });
      });

   AfterEach([this]()
      {
         SourceState->ConditionalBeginDestroy();
      });
}
