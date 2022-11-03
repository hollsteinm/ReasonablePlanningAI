// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "RpaiTypes.generated.h"

DECLARE_STATS_GROUP(TEXT("Rpai"), STATGROUP_Rpai, STATCAT_Advanced);

DECLARE_LOG_CATEGORY_EXTERN(LogRpai, Log, All);

/**
* Represents basic types supported by the ReasonablePlanningState
*/
UENUM(BlueprintType, Category = "Rpai")
enum class EStatePropertyType : uint8
{
	Invalid = 0,
	Bool,
	Class,
	Enum,
	Float,
	Int,
	Name,
	Object,
	Rotator,
	String,
	Vector
};

/**
* References a queryable key value within a ReasonablePlanningState
*/
USTRUCT(BlueprintType, Category = "Rpai")
struct REASONABLEPLANNINGAI_API FStateKeyValueReference
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rpai")
	FName StateKeyName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rpai")
	EStatePropertyType ExpectedValueType;
};

struct FRpaiMemoryView;
struct FRpaiMemorySlice;

/**
* Runtime memory for actions during execution and goals during multi-frame planning. Not Thread Safe
* This is a fast a dirty implementation that is really only meant for internal use. Some things this
* WILL NOT do is the following: 
* - verify your free does not cause a buffer overflow/memory fragmentation
*   due to freeing more or less than what was originally allocated
* - defragment
* - genrally actually allocate or deallocate on the heap, undearneath it is just a TArray<>
* 
* Why?
* Generally a way to transport stateful things much like a BTree Task between service-like and "stateless"
* objects like Actions and Goals. As well as a way to pack up FStruct types in a generic manner.
**/
struct REASONABLEPLANNINGAI_API FRpaiMemory
{
	typedef typename TArray<uint8> StorageType;
	typedef typename StorageType::SizeType MemorySizeType;
private:
	struct Block
	{
	private:
		StorageType AllocatedMemory;
		TArray<TTuple<uint8*, MemorySizeType>> FreeList;
		uint8* Next;

	public:
		Block(MemorySizeType Size);

		/**
		* Returns nullptr on failure to allocate
		**/
		uint8* Allocate(MemorySizeType SizeInBytes, MemorySizeType Alignment);
		void Free(uint8* Memory, MemorySizeType SizeInBytes, MemorySizeType Alignment);
		bool Contains(uint8* Memory) const;
		FORCEINLINE MemorySizeType TotalBytes() const { return AllocatedMemory.Num(); }
	};

	TArray<Block> AllocatedBlocks;
	MemorySizeType SizeOfBlock;

public:
	FRpaiMemory(MemorySizeType BlockSize);

	uint8* Allocate(MemorySizeType SizeInBytes, MemorySizeType Alignment);

	void Free(uint8* Memory, MemorySizeType SizeInBytes, MemorySizeType Alignment);

	FORCEINLINE MemorySizeType GetBlockNum() const { return AllocatedBlocks.Num(); }

	MemorySizeType GetTotalBytesAvailable() const;
};

/**
* Represents a slice of memory from a memory object. Not Thread Safe
**/
struct REASONABLEPLANNINGAI_API FRpaiMemorySlice
{
	typedef typename FRpaiMemory::MemorySizeType MemorySizeType;
private:
	FRpaiMemory* Source;
	uint8* MemoryStart;

public:
	const MemorySizeType ObjectSizeInBytes;
	const MemorySizeType ObjectAlignmentInBytes;
	const MemorySizeType ActualSizeInBytes;


	FRpaiMemorySlice() = delete;
	FRpaiMemorySlice(const FRpaiMemorySlice&) = delete;
	FRpaiMemorySlice& operator=(const FRpaiMemorySlice&) = delete;
	FRpaiMemorySlice(const FRpaiMemorySlice&& OtherView) = delete;

	FRpaiMemorySlice(FRpaiMemory* FromMemory, MemorySizeType SizeInBytes, MemorySizeType AlignmentInBytes);

	~FRpaiMemorySlice();

	template<typename T, MemorySizeType Size = sizeof(T), MemorySizeType Alignment = alignof(T)>
	FRpaiMemorySlice(FRpaiMemory* FromMemory) : FRpaiMemorySlice(FromMemory, Size, Alignment) {}

	template<typename T>
	const T* Get() const
	{
		check(sizeof(T) + (sizeof(T) % alignof(T)) == ActualSizeInBytes);
		return reinterpret_cast<T*>(MemoryStart);
	}

	template<typename T>
	T* Get()
	{
		check(sizeof(T) + (sizeof(T) % alignof(T)) == ActualSizeInBytes);
		return reinterpret_cast<T*>(MemoryStart);
	}

	FORCEINLINE const uint8* GetRaw() const { return MemoryStart; }
	FORCEINLINE uint8* GetRaw() { return MemoryStart; }

	friend FRpaiMemoryView;
};

/**
* ReadOnly version of memory slice. Not Thread Safe
**/
struct REASONABLEPLANNINGAI_API FRpaiMemoryView
{
	typedef typename FRpaiMemorySlice::MemorySizeType MemorySizeType;

	const uint8* MemoryStart;
	const MemorySizeType ActualSizeInBytes;

	FRpaiMemoryView() = delete;

	FRpaiMemoryView(const FRpaiMemoryView& OtherView);
	FRpaiMemoryView(const FRpaiMemoryView&& OtherView) noexcept;
	FRpaiMemoryView(const FRpaiMemorySlice* FromSlice);

	template<typename T>
	const T* Get() const
	{
		check(sizeof(T) + (sizeof(T) % alignof(T)) == ActualSizeInBytes);
		return reinterpret_cast<const T*>(MemoryStart);
	}

	FORCEINLINE const uint8* GetRaw() const { return MemoryStart; }
};

