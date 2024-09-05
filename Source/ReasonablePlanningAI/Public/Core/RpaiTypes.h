// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "PropertyPathHelpers.h"
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
USTRUCT(BlueprintType)
struct REASONABLEPLANNINGAI_API FRpaiMemory
{
	GENERATED_BODY()

	using StorageType = TArray<uint8>;
    using MemorySizeType = StorageType::SizeType;
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
	FRpaiMemory();
	FRpaiMemory(MemorySizeType BlockSize);

	uint8* Allocate(MemorySizeType SizeInBytes, MemorySizeType Alignment);

	void Free(uint8* Memory, MemorySizeType SizeInBytes, MemorySizeType Alignment);

	FORCEINLINE MemorySizeType GetBlockNum() const { return AllocatedBlocks.Num(); }

	MemorySizeType GetTotalBytesAvailable() const;
};

/**
* Represents a slice of memory from a memory object. Not Thread Safe. Ref Counted
**/
USTRUCT(BlueprintType)
struct REASONABLEPLANNINGAI_API FRpaiMemoryStruct
{
	GENERATED_BODY()

	typedef typename FRpaiMemory::MemorySizeType MemorySizeType;

private:
	/**
	* This MUST be first.
	* Allows the following to be equivalent:
	* FRpaiMemoryStruct Instance;
	* void* Container = &Instance;
	* void* ContainerByFirstProperty = &(Instance.MemoryStart)
	* assert(Container == ContainerByFirstProperty
	* 
	* Allows us to abuse pointers to "just hook into" K2Node struct casting using mechanisms similar to the enum casting.
	**/
	uint8* MemoryStart;

	UScriptStruct* Type;

	FRpaiMemory* Source;
	uint32* Refs;

public:
	void AddRef();
	uint32 Release();

	FRpaiMemoryStruct(const FRpaiMemoryStruct&& OtherView) = delete;

	FRpaiMemoryStruct();
	FRpaiMemoryStruct(const FRpaiMemoryStruct& OtherSlice);
	FRpaiMemoryStruct(FRpaiMemoryStruct&& OtherSlice);
	FRpaiMemoryStruct& operator=(const FRpaiMemoryStruct& OtherSlice);
	FRpaiMemoryStruct(FRpaiMemory* FromMemory, UScriptStruct* FromStructType);

	~FRpaiMemoryStruct();

	bool IsCompatibleType(const UScriptStruct* TestType) const;

	FORCEINLINE bool IsValid() const { return Refs != nullptr && MemoryStart != nullptr; }

	template<typename TScriptStructType>
	FORCEINLINE bool IsCompatibleTypeOf() const { return IsCompatibleType(TScriptStructType::StaticStruct()); }

	template<typename TScriptStructType>
	static FRpaiMemoryStruct ForType(FRpaiMemory* FromMemory) { return FRpaiMemoryStruct(FromMemory, TScriptStructType::StaticStruct()); }

	template<typename T>
	const T* Get() const
	{
		check(IsValid());
		check(T::StaticStruct() == Type);
		return reinterpret_cast<T*>(MemoryStart);
	}

	template<typename T>
	T* Get()
	{
		check(IsValid());
		check(T::StaticStruct() == Type);
		return reinterpret_cast<T*>(MemoryStart);
	}

	FORCEINLINE const uint8* GetRaw() const { return MemoryStart; }
	FORCEINLINE uint8* GetRaw() { return MemoryStart; }

	FORCEINLINE UScriptStruct* GetType() const { return Type; }

	template<typename T>
	void Set(T NewValue)
	{
		check(T::StaticStruct() == Type);
		FMemory::Memcpy(static_cast<void*>(MemoryStart), static_cast<const void*>(&NewValue), Type->GetStructureSize());
	}
};

UENUM(BlueprintType)
enum class ERpaiPlannerResult : uint8
{
	CompletedFailure,
	CompletedCancelled,
	CompletedSuccess,
	RequiresTick,
	Invalid
};

// V2
USTRUCT(BlueprintType)
struct REASONABLEPLANNINGAI_API FStateSourcePropertyHandle
{
	GENERATED_BODY()

	UPROPERTY()
	int32 Handle;

	FORCEINLINE bool IsValid() const { return Handle != INDEX_NONE; }
};

USTRUCT(BlueprintType)
struct REASONABLEPLANNINGAI_API FStateSourceTypeHandle
{
	GENERATED_BODY()

	UPROPERTY()
	int32 Handle;

	FORCEINLINE bool IsValid() const { return Handle != INDEX_NONE; }
};

USTRUCT(BlueprintType)
struct REASONABLEPLANNINGAI_API FStateTargetPropertyHandle
{
	GENERATED_BODY()

	UPROPERTY()
	int32 Handle;

	FORCEINLINE bool IsValid() const { return Handle != INDEX_NONE; }
};

USTRUCT(BlueprintType)
struct REASONABLEPLANNINGAI_API FStateTargetTypeHandle
{
	GENERATED_BODY()

	UPROPERTY()
	int32 Handle;

	FORCEINLINE bool IsValid() const { return Handle != INDEX_NONE; }
};

USTRUCT(BlueprintType)
struct REASONABLEPLANNINGAI_API FStateBindingHandle
{
	GENERATED_BODY()

	UPROPERTY()
	FStateSourceTypeHandle StateSourceTypeHandle;

	UPROPERTY()
	FStateSourcePropertyHandle StateSourcePropertyHandle;

	UPROPERTY()
	FStateTargetTypeHandle StateTargetTypeHandle;

	UPROPERTY()
	FStateTargetPropertyHandle StateTargetPropertyHandle;

	UPROPERTY()
	int32 Handle;

	FORCEINLINE bool IsValid() const {
		return
			Handle != INDEX_NONE &&
			StateSourceTypeHandle.IsValid() &&
			StateSourcePropertyHandle.IsValid() &&
			StateTargetTypeHandle.IsValid() &&
			StateTargetPropertyHandle.IsValid();
	}

	// TODO: Add C++ notification callbacks for rebinds and resorts
};

USTRUCT(BlueprintType)
struct FRpaiCachedPropertyPath : public FCachedPropertyPath
{
    GENERATED_BODY()
    
    REASONABLEPLANNINGAI_API FRpaiCachedPropertyPath();

    REASONABLEPLANNINGAI_API FRpaiCachedPropertyPath(const FString& Path);

    REASONABLEPLANNINGAI_API FRpaiCachedPropertyPath(const TArray<FString>& PropertyChain);

};

USTRUCT(BlueprintType)
struct REASONABLEPLANNINGAI_API FRpaiStateTypePropertyMultiBind
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category=Rpai)
	TObjectPtr<UStruct> TargetBindingClass;

    UPROPERTY(EditAnywhere, Category=Rpai)
	TArray<FRpaiCachedPropertyPath> BoundProperties;

	int32 AddBinding(const FString& PropertyName);

	void RemoveBinding(int32 BindingHandle);
};

USTRUCT(BlueprintType)
struct REASONABLEPLANNINGAI_API FRpaiStateBinding
{
	GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category=Rpai)
	TArray<FRpaiStateTypePropertyMultiBind> SourceBindings;
    
    UPROPERTY(EditAnywhere, Category=Rpai)
	TArray<FRpaiStateTypePropertyMultiBind> TargetBindings;

	UPROPERTY(EditAnywhere, Category=Rpai)
	TArray<FStateBindingHandle> BindingHandles;

	FStateBindingHandle AddBinding(TObjectPtr<UStruct> SourceType, const FString& SourcePropertyName, TObjectPtr<UStruct> TargetType, const FString& TargetPropertyName);
	void RemoveBinding(const FStateBindingHandle& Handle);
	
	bool Transfer(const UObject* Source, FRpaiMemoryStruct& Target) const;
    bool Transfer(const UObject* Source, uint8* TargetData, UScriptStruct* TargetStructType) const;
    bool Transfer(const UObject* Source, UObject* Target) const;
    bool Transfer(const AActor* Source, UObject* Target) const;
};
