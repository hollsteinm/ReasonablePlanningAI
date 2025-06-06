// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Core/RpaiTypes.h"
#include "ReasonablePlanningAITestTypes.h"


BEGIN_DEFINE_SPEC(ReasonablePlanningMemoryTests, "ReasonablePlanningAI.Core.Memory", EAutomationTestFlags::ProductFilter | EAutomationTestFlags_ApplicationContextMask)
END_DEFINE_SPEC(ReasonablePlanningMemoryTests)
void ReasonablePlanningMemoryTests::Define()
{
   Describe("Memory Allocation & Freeing", [this]()
      {
         It("should have a default state of one block", [this]()
            {
               const FRpaiMemory::MemorySizeType BlockSize = 256;
               FRpaiMemory ClassUnderTest(BlockSize);

               TestEqual("FRpaiMemory::FRpaiMemory", ClassUnderTest.GetBlockNum(), 1);
               TestEqual("FRpaiMemory::FRpaiMemory", ClassUnderTest.GetTotalBytesAvailable(), BlockSize);
            });

         It("should allocate memory within block size", [this]()
            {
               const int32 BlockSize = 128;
               FRpaiMemory ClassUnderTest(BlockSize);

               uint8* Allocated = ClassUnderTest.Allocate(10, 8); // Should allocate 16 bytes
               TestNotNull("FRpaiMemory::Allocate", Allocated);

               ClassUnderTest.Free(Allocated, 10, 8);
            });

         It("should allocate memory outside block size", [this]()
            {
               const int32 BlockSize = 8;
               FRpaiMemory ClassUnderTest(BlockSize);

               uint8* Allocated = ClassUnderTest.Allocate(32, 8);
               TestNotNull("FRpaiMemory::Allocate", Allocated);

               ClassUnderTest.Free(Allocated, 32, 8);
            });
      });

   Describe("Memory Slice", [this]()
      {
         It("should create a slice with appropriate allocations", [this]()
            {
               FRpaiMemory TestMemory(128);
               FRpaiMemoryStruct ClassUnderTest(&TestMemory, FTestStruct::StaticStruct());

               TestNotNull("Memory Not Null", ClassUnderTest.GetRaw());
            });
      });
}