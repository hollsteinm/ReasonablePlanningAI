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
			Next = Next + AllocationSize;
			return AllocationToReturn;
		}
	}

	return nullptr;
}

void FRpaiMemory::Block::Free(uint8* Memory, FRpaiMemory::MemorySizeType SizeInBytes, FRpaiMemory::MemorySizeType Alignment)
{
	FreeList.Add(MakeTuple(Memory, SizeInBytes));
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

FRpaiMemorySlice::FRpaiMemorySlice(FRpaiMemory* FromMemory, FRpaiMemorySlice::MemorySizeType SizeInBytes, FRpaiMemorySlice::MemorySizeType AlignmentInBytes)
	: Source(FromMemory)
	, ObjectSizeInBytes(SizeInBytes)
	, ObjectAlignmentInBytes(AlignmentInBytes)
	, ActualSizeInBytes(SizeInBytes + (SizeInBytes % AlignmentInBytes))
{
	MemoryStart = FromMemory->Allocate(ObjectSizeInBytes, ObjectAlignmentInBytes);
}

FRpaiMemorySlice::~FRpaiMemorySlice()
{
	Source->Free(MemoryStart, ObjectSizeInBytes, ObjectAlignmentInBytes);
}

FRpaiMemoryView::FRpaiMemoryView(const FRpaiMemoryView& OtherView)
	: MemoryStart(OtherView.MemoryStart)
	, ActualSizeInBytes(OtherView.ActualSizeInBytes)
{

}

FRpaiMemoryView::FRpaiMemoryView(const FRpaiMemoryView&& OtherView) noexcept
	: MemoryStart(OtherView.MemoryStart)
	, ActualSizeInBytes(OtherView.ActualSizeInBytes)
{

}

FRpaiMemoryView::FRpaiMemoryView(const FRpaiMemorySlice* FromSlice)
	: MemoryStart(FromSlice->MemoryStart)
	, ActualSizeInBytes(FromSlice->ActualSizeInBytes)
{

}