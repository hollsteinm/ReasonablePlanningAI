// Troll Purse. All rights reserved.


#include "RpaiBPLibrary.h"

bool URpaiBPLibrary::IsSafeToReadAs(const FRpaiMemoryStruct& Memory, const UScriptStruct* StructType)
{
	return Memory.IsCompatibleType(StructType);
}

bool URpaiBPLibrary::ReadMemory(FRpaiMemoryStruct& Table, FRpaiPlaceholderStruct& OutStruct)
{
	check(false);
	return false;
}