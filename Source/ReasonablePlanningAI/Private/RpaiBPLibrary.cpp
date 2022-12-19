// Troll Purse. All rights reserved.


#include "RpaiBPLibrary.h"

bool URpaiBPLibrary::IsSafeToReadAs(const FRpaiMemoryStruct& Memory, const UScriptStruct* StructType)
{
	return Memory.IsOfType(StructType);
}