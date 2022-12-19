// Troll Purse. All rights reserved.


#include "Core/RpaiTypes.h"

DEFINE_LOG_CATEGORY(LogRpai)

FRpaiMemory::Block::Block(FRpaiMemory::MemorySizeType Size)
	: AllocatedMemory()
	, FreeList()
	, Next(nullptr)
{
	AllocatedMemory.AddUninitialized(Size);
	Next = &AllocatedMemory[0];
}

bool FRpaiMemory::Block::Contains(uint8* Memory) const
{
	return Memory >= &AllocatedMemory[0] && Memory <= &AllocatedMemory[AllocatedMemory.Num() - 1];
}

uint8* FRpaiMemory::Block::Allocate(FRpaiMemory::MemorySizeType SizeInBytes, FRpaiMemory::MemorySizeType Alignment)
{
	FRpaiMemory::MemorySizeType AllocationSize = SizeInBytes + (SizeInBytes % Alignment);
	FRpaiMemory::MemorySizeType FreeListNum = FreeList.Num();
	if (FreeListNum > 0)
	{
		for (FRpaiMemory::MemorySizeType Index = 0; Index < FreeListNum; ++Index)
		{
			auto& Entry = FreeList[0];
			// Exact fit only, prevent fragmentation due to
			// smaller and smaller allocations on larger
			// freed blocks as we don't have a means to
			// reclaim here. Going for CPU speed.
			if (Entry.Get<1>() == AllocationSize)
			{
				uint8* AllocationToReturn = Entry.Get<0>();
				FreeList.RemoveAt(Index);
				return AllocationToReturn;
			}
		}
	} // Allow Fall through if no matching sizes in the free list

	const uint8* End = &AllocatedMemory[AllocatedMemory.Num() - 1] + 1;
	if (Next < End)
	{
		uint8* NextToAllocEnd = Next + AllocationSize;
		if (NextToAllocEnd <= End)
		{
			uint8* AllocationToReturn = Next;
			Next = Next + AllocationSize + 1;
			return AllocationToReturn;
		}
	}

	return nullptr;
}

void FRpaiMemory::Block::Free(uint8* Memory, FRpaiMemory::MemorySizeType SizeInBytes, FRpaiMemory::MemorySizeType Alignment)
{
	FreeList.Add(MakeTuple(Memory, SizeInBytes));
}

FRpaiMemory::FRpaiMemory()
	: FRpaiMemory(32)
{
}

FRpaiMemory::FRpaiMemory(FRpaiMemory::MemorySizeType BlockSize)
	: AllocatedBlocks()
	, SizeOfBlock(BlockSize)
{
	AllocatedBlocks.Add(FRpaiMemory::Block(SizeOfBlock));
}

uint8* FRpaiMemory::Allocate(FRpaiMemory::MemorySizeType SizeInBytes, FRpaiMemory::MemorySizeType Alignment)
{
	uint8* Allocation = nullptr;
	// Forward iteration to attempt free list allocation over new block allocation
	for (auto& AllocatedBlock : AllocatedBlocks)
	{
		Allocation = AllocatedBlock.Allocate(SizeInBytes, Alignment);
		if (Allocation != nullptr)
		{
			return Allocation;
		}
	} // If we fall through no allocation was found

	int32 NewIdx = AllocatedBlocks.Add(FRpaiMemory::Block(FMath::Max(SizeOfBlock, SizeInBytes + (SizeInBytes % Alignment))));
	Allocation = AllocatedBlocks[NewIdx].Allocate(SizeInBytes, Alignment);
	return Allocation;
}

void FRpaiMemory::Free(uint8* Memory, FRpaiMemory::MemorySizeType SizeInBytes, FRpaiMemory::MemorySizeType Alignment)
{
	TFunction<bool(FRpaiMemory::Block&)> Predicate = [Memory](const FRpaiMemory::Block& Block) -> bool { return Block.Contains(Memory); };
	auto OwningBlock = AllocatedBlocks.FindByPredicate(Predicate);
	if (LIKELY(OwningBlock != nullptr))
	{
		OwningBlock->Free(Memory, SizeInBytes, Alignment);
	}
}

FRpaiMemory::MemorySizeType FRpaiMemory::GetTotalBytesAvailable() const
{
	FRpaiMemory::MemorySizeType Total = 0;
	for (const auto& Block : AllocatedBlocks)
	{
		Total += Block.TotalBytes();
	}
	return Total;
}

FRpaiMemoryStruct::FRpaiMemoryStruct()
	: Source(nullptr)
	, Refs(nullptr)
	, Type(nullptr)
	, MemoryStart(nullptr)
{
}

FRpaiMemoryStruct::FRpaiMemoryStruct(FRpaiMemory* FromMemory, UScriptStruct* FromStructType)
	: Source(FromMemory)
	, Refs(static_cast<uint32*>(FMemory::Malloc(sizeof(uint32))))
	, Type(FromStructType)
{
	*Refs = 0;
	MemoryStart = FromMemory->Allocate(Type->GetStructureSize(), Type->GetMinAlignment());
	FMemory::Memzero(MemoryStart, Type->GetStructureSize());

	auto CppOpts = Type->GetCppStructOps();
	if (CppOpts != nullptr && !CppOpts->HasZeroConstructor())
	{
		CppOpts->Construct(MemoryStart);
	}
	AddRef();
}

FRpaiMemoryStruct::FRpaiMemoryStruct(const FRpaiMemoryStruct& OtherSlice)
	: Source(OtherSlice.Source)
	, MemoryStart(OtherSlice.MemoryStart)
	, Refs(OtherSlice.Refs)
	, Type(OtherSlice.Type)
{
	AddRef();
}

FRpaiMemoryStruct::FRpaiMemoryStruct(FRpaiMemoryStruct&& OtherSlice)
	: FRpaiMemoryStruct()
{
	Swap(Source, OtherSlice.Source);
	Swap(MemoryStart, OtherSlice.MemoryStart);
	Swap(Refs, OtherSlice.Refs);
	Swap(Type, OtherSlice.Type);
}

FRpaiMemoryStruct& FRpaiMemoryStruct::operator=(const FRpaiMemoryStruct& OtherSlice)
{
	if (this != &OtherSlice)
	{
		if (Release() == 0)
		{
			FMemory::Free(Refs);
			Refs = nullptr;
			if (LIKELY(Source != nullptr))
			{
				if (LIKELY(Type != nullptr))
				{
					Type->DestroyStruct(MemoryStart);
				}
				Source->Free(MemoryStart, Type->GetStructureSize(), Type->GetMinAlignment());
				Source = nullptr;
			}
		}

		Source = OtherSlice.Source;
		MemoryStart = OtherSlice.MemoryStart;
		Refs = OtherSlice.Refs;
		Type = OtherSlice.Type;
		AddRef();
	}
	return *this;
}

void FRpaiMemoryStruct::AddRef()
{
	if (Refs != nullptr)
	{
		++(*Refs);
	}
}

uint32 FRpaiMemoryStruct::Release()
{
	return Refs == nullptr ? INDEX_NONE : --(*Refs);
}

FRpaiMemoryStruct::~FRpaiMemoryStruct()
{
	if (Release() == 0)
	{
		FMemory::Free(Refs);
		Refs = nullptr;
		if (LIKELY(Source != nullptr))
		{
			if (LIKELY(Type != nullptr))
			{
				auto* CppOpts = Type->GetCppStructOps();
				if (CppOpts != nullptr && CppOpts->HasDestructor())
				{
					CppOpts->Destruct(MemoryStart);
				}
				Source->Free(MemoryStart, Type->GetStructureSize(), Type->GetMinAlignment());
				MemoryStart = nullptr;
			}
			Source = nullptr;
		}
		ensure(MemoryStart == nullptr);
	}
}
