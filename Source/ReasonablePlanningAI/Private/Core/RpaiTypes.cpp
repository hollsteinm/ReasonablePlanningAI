// Radaway Software LLC. 2025. All rights reserved.


#include "Core/RpaiTypes.h"
#include "PropertyPathHelpers.h"

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
	if (AllocatedMemory.IsEmpty())
	{
		return false;
	}
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
	: MemoryStart(nullptr)
	, Type(nullptr)
	, Source(nullptr)
	, Refs(nullptr)
{
}

FRpaiMemoryStruct::FRpaiMemoryStruct(FRpaiMemory* FromMemory, UScriptStruct* FromStructType)
	: MemoryStart(FromMemory->Allocate(FromStructType->GetStructureSize(), FromStructType->GetMinAlignment()))
    , Type(FromStructType)
    , Source(FromMemory)
	, Refs(static_cast<uint32*>(FMemory::Malloc(sizeof(uint32))))
{
	FMemory::Memzero(Refs, sizeof(uint32));
	FMemory::Memzero(MemoryStart, Type->GetStructureSize());

	auto CppOpts = Type->GetCppStructOps();
	if (CppOpts != nullptr && !CppOpts->HasZeroConstructor())
	{
		CppOpts->Construct(MemoryStart);
	}
	else
	{
		Type->InitializeDefaultValue(MemoryStart);
	}

	/**
	* The intended uses of the struct types loaded by this "memory manager" is to provide
	* dynamic creation and storage of structs in both C++ and Blueprints. A consequence of this
	* is that the USTRUCT never really makes an appearance in the Asset, Editor, or Game CDOs or 
	* objects tracked for garbage collection. This means that if a USTRUCT used in this manner contains
	* a strong pointer to a UObject (via UPROPERTY or TSharedPtr and fam) it will not actually get
	* tracked properly by the GC, but for all intents it is a valid object. IsValid calls will be true but
	* IsUnreachable will also be true! By post loading here we can ensure that the proper object references
	* are tracked. This keeps it simple for compatible experience across C++ and Blueprints.
	**/
	Type->PostLoad();

	AddRef();
}

FRpaiMemoryStruct::FRpaiMemoryStruct(const FRpaiMemoryStruct& OtherSlice)
    : MemoryStart(OtherSlice.MemoryStart) 
    , Type(OtherSlice.Type)
    , Source(OtherSlice.Source)
    , Refs(OtherSlice.Refs)
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
				if (LIKELY(Type != nullptr) && LIKELY(MemoryStart != nullptr))
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
		ensure(*Refs < TNumericLimits<uint32>::Max());
	}
	else
	{
		ensure(MemoryStart == nullptr);
		ensure(Source == nullptr);
		ensure(Type == nullptr);
		ensure(Refs == nullptr);
	}
}

uint32 FRpaiMemoryStruct::Release()
{
	//Defualt Ctor allows uninitalized reference. A reference to nothing. This can occur during smoke tests.
	if (Refs != nullptr)
	{
		return --(*Refs);
	}
	return TNumericLimits<uint32>::Max();
}

bool FRpaiMemoryStruct::IsCompatibleType(const UScriptStruct* TestType) const
{
	return (TestType == Type) || FStructUtils::TheSameLayout(TestType, Type);
}

FRpaiMemoryStruct::~FRpaiMemoryStruct()
{
	uint32 RemainingRefs = Release();
	if (RemainingRefs == 0)
	{
		FMemory::Free(Refs);
		Refs = nullptr;
		if (LIKELY(Source != nullptr))
		{
			if (LIKELY(Type != nullptr))
			{
				Source->Free(MemoryStart, Type->GetStructureSize(), Type->GetMinAlignment());
				MemoryStart = nullptr;
				Type = nullptr;
			}
			Source = nullptr;
		}
	}
	if (RemainingRefs == TNumericLimits<uint32>::Max() || RemainingRefs == 0)
	{
		ensure(MemoryStart == nullptr);
		ensure(Source == nullptr);
		ensure(Type == nullptr);
		ensure(Refs == nullptr);
	}
}

FRpaiCachedPropertyPath::FRpaiCachedPropertyPath()
: FCachedPropertyPath()
{
    
}

FRpaiCachedPropertyPath::FRpaiCachedPropertyPath(const FString& Path)
: FCachedPropertyPath(Path)
{
    
}

FRpaiCachedPropertyPath::FRpaiCachedPropertyPath(const TArray<FString>& PropertyChain)
: FCachedPropertyPath(PropertyChain)
{
    
}


int32 FRpaiStateTypePropertyMultiBind::AddBinding(const FString& PropertyName)
{
	if (IsValid(TargetBindingClass))
	{
        FRpaiCachedPropertyPath Property(PropertyName);
        return BoundProperties.Add(Property);
	}
	else
	{
		return INDEX_NONE;
	}
}

void FRpaiStateTypePropertyMultiBind::RemoveBinding(int32 BindingHandle)
{
	if (BoundProperties.IsValidIndex(BindingHandle))
	{
		BoundProperties.RemoveAt(BindingHandle, 1, false);
	}
}

FStateBindingHandle FRpaiStateBinding::AddBinding(TObjectPtr<UStruct> SourceType, const FString& SourcePropertyName, TObjectPtr<UStruct> TargetType, const FString& TargetPropertyName)
{
	int32 SourceTypeIndex = INDEX_NONE;
	int32 TargetTypeIndex = INDEX_NONE;

	for (int32 Idx = 0; Idx < SourceBindings.Num(); ++Idx)
	{
		if (SourceType->IsChildOf(SourceBindings[Idx].TargetBindingClass))
		{
			SourceTypeIndex = Idx;
			break;
		}
	}

	for (int32 Idx = 0; Idx < TargetBindings.Num(); ++Idx)
	{
		if (TargetType->IsChildOf(TargetBindings[Idx].TargetBindingClass))
		{
			TargetTypeIndex = Idx;
			break;
		}
	}

	if (SourceTypeIndex == INDEX_NONE)
	{
		SourceTypeIndex = SourceBindings.AddDefaulted();
		SourceBindings[SourceTypeIndex].TargetBindingClass = SourceType;
	}

	if (TargetTypeIndex == INDEX_NONE)
	{
		TargetTypeIndex = TargetBindings.AddDefaulted();
		TargetBindings[TargetTypeIndex].TargetBindingClass = TargetType;
	}

	int32 SourcePropertyBindingIndex = SourceBindings[SourceTypeIndex].AddBinding(SourcePropertyName);
	int32 TargetPropertyBindingIndex = TargetBindings[TargetTypeIndex].AddBinding(TargetPropertyName);

	FStateBindingHandle& Binding = BindingHandles.AddDefaulted_GetRef();
	Binding.Handle = BindingHandles.Num() - 1;
	Binding.StateSourceTypeHandle.Handle = SourceTypeIndex;
	Binding.StateSourcePropertyHandle.Handle = SourcePropertyBindingIndex;
	Binding.StateTargetTypeHandle.Handle = TargetTypeIndex;
	Binding.StateTargetPropertyHandle.Handle = TargetPropertyBindingIndex;
	return Binding;
}

void FRpaiStateBinding::RemoveBinding(const FStateBindingHandle& Handle)
{
	if (BindingHandles.IsValidIndex(Handle.Handle))
	{
		if (SourceBindings.IsValidIndex(Handle.StateSourceTypeHandle.Handle) && TargetBindings.IsValidIndex(Handle.StateTargetTypeHandle.Handle))
		{
			SourceBindings[Handle.StateSourceTypeHandle.Handle].RemoveBinding(Handle.StateSourcePropertyHandle.Handle);
			TargetBindings[Handle.StateTargetTypeHandle.Handle].RemoveBinding(Handle.StateTargetPropertyHandle.Handle);
		}

		if (SourceBindings[Handle.StateSourceTypeHandle.Handle].BoundProperties.IsEmpty())
		{
			SourceBindings.RemoveAt(Handle.StateSourceTypeHandle.Handle, 1, false);
		}

		if (TargetBindings[Handle.StateTargetTypeHandle.Handle].BoundProperties.IsEmpty())
		{
			TargetBindings.RemoveAt(Handle.StateTargetTypeHandle.Handle, 1, false);
		}

		BindingHandles.RemoveAt(Handle.Handle, 1, false);
	}
}

bool FRpaiStateBinding::Transfer(const UObject* Source, FRpaiMemoryStruct& Target) const
{
    check(Source);
    check(Target.GetRaw());
    int32 SourceTypeIndex = INDEX_NONE;
    int32 TargetTypeIndex = INDEX_NONE;

	for (int32 Idx = 0; Idx < SourceBindings.Num(); ++Idx)
	{
		if (Source->GetClass()->IsChildOf(SourceBindings[Idx].TargetBindingClass))
		{
            SourceTypeIndex = Idx;
			break;
		}
	}

	for (int32 Idx = 0; Idx < TargetBindings.Num(); ++Idx)
	{
		if (Target.GetType()->IsChildOf(TargetBindings[Idx].TargetBindingClass))
		{
            TargetTypeIndex = Idx;
			break;
		}
	}

	if(SourceTypeIndex == INDEX_NONE || TargetTypeIndex == INDEX_NONE)
    {
        return false;
    }

    UStruct* SourceType = SourceBindings[SourceTypeIndex].TargetBindingClass;
    UStruct* TargetType = TargetBindings[TargetTypeIndex].TargetBindingClass;
    TArray<FRpaiCachedPropertyPath> SourceProperties = SourceBindings[SourceTypeIndex].BoundProperties;
    TArray<FRpaiCachedPropertyPath> TargetProperties = TargetBindings[TargetTypeIndex].BoundProperties;
	int32 CopyCount = FMath::Min(SourceProperties.Num(), TargetProperties.Num());
	for (int32 Idx = 0; Idx < CopyCount; ++Idx)
	{
        FRpaiCachedPropertyPath SourceProperty = SourceProperties[Idx];
        FRpaiCachedPropertyPath TargetProperty = TargetProperties[Idx];
        UObject* SourcePtr = const_cast<UObject*>(Source);
        uint8* TargetPtr = const_cast<uint8*>(Target.GetRaw());
        FString SourceStringValue;
        if(PropertyPathHelpers::GetPropertyValueAsString(SourcePtr, SourceType, SourceProperty, SourceStringValue))
        {
            PropertyPathHelpers::SetPropertyValueFromString(TargetPtr, TargetType, TargetProperty, SourceStringValue);
        }
	}

	return true;
}

bool FRpaiStateBinding::Transfer(const UObject* Source, uint8* TargetData, UScriptStruct* TargetStructType) const
{
    check(Source);
    check(TargetStructType);
    
    int32 SourceTypeIndex = INDEX_NONE;
    int32 TargetTypeIndex = INDEX_NONE;

    for (int32 Idx = 0; Idx < SourceBindings.Num(); ++Idx)
    {
        if (Source->GetClass()->IsChildOf(SourceBindings[Idx].TargetBindingClass))
        {
            SourceTypeIndex = Idx;
            break;
        }
    }

    for (int32 Idx = 0; Idx < TargetBindings.Num(); ++Idx)
    {
        if (TargetStructType->IsChildOf(TargetBindings[Idx].TargetBindingClass))
        {
            TargetTypeIndex = Idx;
            break;
        }
    }

    if(SourceTypeIndex == INDEX_NONE || TargetTypeIndex == INDEX_NONE)
    {
        return false;
    }

    UStruct* SourceType = SourceBindings[SourceTypeIndex].TargetBindingClass;
    UStruct* TargetType = TargetBindings[TargetTypeIndex].TargetBindingClass;
    TArray<FRpaiCachedPropertyPath> SourceProperties = SourceBindings[SourceTypeIndex].BoundProperties;
    TArray<FRpaiCachedPropertyPath> TargetProperties = TargetBindings[TargetTypeIndex].BoundProperties;
    int32 CopyCount = FMath::Min(SourceProperties.Num(), TargetProperties.Num());
    for (int32 Idx = 0; Idx < CopyCount; ++Idx)
    {
        FRpaiCachedPropertyPath SourceProperty = SourceProperties[Idx];
        FRpaiCachedPropertyPath TargetProperty = TargetProperties[Idx];
        UObject* SourcePtr = const_cast<UObject*>(Source);
        uint8* TargetPtr = TargetData;
        FString SourceStringValue;
        if(PropertyPathHelpers::GetPropertyValueAsString(SourcePtr, SourceType, SourceProperty, SourceStringValue))
        {
            PropertyPathHelpers::SetPropertyValueFromString(TargetPtr, TargetType, TargetProperty, SourceStringValue);
        }
    }

    return true;
}

bool FRpaiStateBinding::Transfer(const UObject* Source, UObject* Target) const
{
    check(Source);
    check(Target);
    int32 SourceTypeIndex = INDEX_NONE;
    int32 TargetTypeIndex = INDEX_NONE;

    for (int32 Idx = 0; Idx < SourceBindings.Num(); ++Idx)
    {
        if (Source->GetClass()->IsChildOf(SourceBindings[Idx].TargetBindingClass))
        {
            SourceTypeIndex = Idx;
            break;
        }
    }

    for (int32 Idx = 0; Idx < TargetBindings.Num(); ++Idx)
    {
        if (Target->GetClass()->IsChildOf(TargetBindings[Idx].TargetBindingClass))
        {
            TargetTypeIndex = Idx;
            break;
        }
    }

    if(SourceTypeIndex == INDEX_NONE || TargetTypeIndex == INDEX_NONE)
    {
        return false;
    }

    UStruct* SourceType = SourceBindings[SourceTypeIndex].TargetBindingClass;
    UStruct* TargetType = TargetBindings[TargetTypeIndex].TargetBindingClass;
    TArray<FRpaiCachedPropertyPath> SourceProperties = SourceBindings[SourceTypeIndex].BoundProperties;
    TArray<FRpaiCachedPropertyPath> TargetProperties = TargetBindings[TargetTypeIndex].BoundProperties;
    int32 CopyCount = FMath::Min(SourceProperties.Num(), TargetProperties.Num());
    for (int32 Idx = 0; Idx < CopyCount; ++Idx)
    {
        FRpaiCachedPropertyPath SourceProperty = SourceProperties[Idx];
        FRpaiCachedPropertyPath TargetProperty = TargetProperties[Idx];
        UObject* SourcePtr = const_cast<UObject*>(Source);
        FString SourceStringValue;
        if(PropertyPathHelpers::GetPropertyValueAsString(SourcePtr, SourceType, SourceProperty, SourceStringValue))
        {
            if(!PropertyPathHelpers::SetPropertyValueFromString(Target, TargetType, TargetProperty, SourceStringValue))
            {
                UE_LOG(LogRpai, Warning, TEXT("Unable to set target Property Path %s "), *TargetProperty.ToString());
            }
        }
        else
        {
            UE_LOG(LogRpai, Warning, TEXT("Unable to get source Property Path %s "), *SourceProperty.ToString());
        }
    }

    return true;
}

bool FRpaiStateBinding::Transfer(const AActor* Source, UObject* Target) const
{
    const UObject* AsUObject = Cast<UObject>(Source);
    return Transfer(AsUObject, Target);
}
