// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.


#include "RpaiBPLibrary.h"

bool URpaiBPLibrary::IsSafeToReadAs(const FRpaiMemoryStruct& Memory, const UScriptStruct* StructType)
{
   return Memory.IsCompatibleType(StructType);
}

bool URpaiBPLibrary::ReadMemory(const FRpaiMemoryStruct& Memory, FRpaiPlaceholderStruct& OutStruct)
{
   check(false);
   return false;
}

bool URpaiBPLibrary::WriteMemory(FRpaiMemoryStruct& Memory, FRpaiPlaceholderStruct InStruct)
{
   check(false);
   return false;
}