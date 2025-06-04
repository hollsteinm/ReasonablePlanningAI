// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Core/RpaiTypes.h"
#include "Misc/EngineVersionComparison.h"
#if UE_VERSION_NEWER_THAN(5,3,2)
#include "Blueprint/BlueprintExceptionInfo.h"
#endif
#include "RpaiBPLibrary.generated.h"

class URpaiState;

USTRUCT(BlueprintInternalUseOnly)
struct FRpaiPlaceholderStruct
{
   GENERATED_USTRUCT_BODY()
};

/**
 * Blueprint function library for Reasonable Planning AI Plugin.
 */
UCLASS()
class REASONABLEPLANNINGAI_API URpaiBPLibrary : public UBlueprintFunctionLibrary
{
   GENERATED_BODY()

public:
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Rpai")
   static bool IsSafeToReadAs(UPARAM(Ref) const FRpaiMemoryStruct& Memory, const UScriptStruct* StructType);

   UFUNCTION(BlueprintCallable, CustomThunk, Category = "Rpai", meta = (CustomStructureParam = "OutStruct", BlueprintInternalUseOnly = "true"))
   static bool ReadMemory(UPARAM(Ref) const FRpaiMemoryStruct& Memory, FRpaiPlaceholderStruct& OutStruct);
   
   UFUNCTION(BlueprintCallable, CustomThunk, Category = "Rpai", meta = (CustomStructureParam = "InStruct", BlueprintInternalUseOnly = "true"))
   static bool WriteMemory(UPARAM(Ref) FRpaiMemoryStruct& Memory, FRpaiPlaceholderStruct InStruct);

   DECLARE_FUNCTION(execReadMemory)
   {
      P_GET_STRUCT_REF(FRpaiMemoryStruct, Memory);

      Stack.StepCompiledIn<FStructProperty>(NULL);
      void* OutRowPtr = Stack.MostRecentPropertyAddress;

      P_FINISH;

      bool bSuccess = false;
      auto Raw = Memory.GetRaw();
      if (Raw == nullptr)
      {
         FBlueprintExceptionInfo ExceptionInfo(
            EBlueprintExceptionType::AccessViolation,
            NSLOCTEXT("Rpai", "NullMemory", "Memory was null.")
         );
         FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ExceptionInfo);
      }
      else
      {
         FStructProperty* StructProp = CastField<FStructProperty>(Stack.MostRecentProperty);
         if (StructProp && OutRowPtr)
         {
            const UScriptStruct* OutputType = StructProp->Struct;
            const UScriptStruct* EncapsulatedMemoryType = Memory.GetType();

            if (Memory.IsCompatibleType(OutputType))
            {
               P_NATIVE_BEGIN;
               EncapsulatedMemoryType->CopyScriptStruct(OutRowPtr, Raw);
               P_NATIVE_END;
               bSuccess = true;
            }
            else
            {
               FBlueprintExceptionInfo ExceptionInfo(
                  EBlueprintExceptionType::AccessViolation,
                  NSLOCTEXT("Rpai", "IncompatibleStructType", "Incompatible output parameter; the memory's type is not the same as the return type.")
               );
               FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ExceptionInfo);
            }
         }
         else
         {
            FBlueprintExceptionInfo ExceptionInfo(
               EBlueprintExceptionType::AccessViolation,
               NSLOCTEXT("Rpai", "MissingType", "Failed to resolve the output parameter for ReadAsStruct.")
            );
            FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ExceptionInfo);
         }
      }
      *(bool*)RESULT_PARAM = bSuccess;
   }

   DECLARE_FUNCTION(execWriteMemory)
   {
      P_GET_STRUCT_REF(FRpaiMemoryStruct, Memory);

      Stack.StepCompiledIn<FStructProperty>(NULL);
      void* InDataPtr = Stack.MostRecentPropertyAddress;

      P_FINISH;

      bool bSuccess = false;
      auto Raw = Memory.GetRaw();
      if (Raw == nullptr)
      {
         FBlueprintExceptionInfo ExceptionInfo(
            EBlueprintExceptionType::AccessViolation,
            NSLOCTEXT("Rpai", "NullMemory", "Memory was null.")
         );
         FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ExceptionInfo);
      }
      else
      {
         FStructProperty* StructProp = CastField<FStructProperty>(Stack.MostRecentProperty);
         if (StructProp && InDataPtr)
         {
            const UScriptStruct* OutputType = StructProp->Struct;
            const UScriptStruct* EncapsulatedMemoryType = Memory.GetType();

            if (Memory.IsCompatibleType(OutputType))
            {
               P_NATIVE_BEGIN;
               EncapsulatedMemoryType->CopyScriptStruct(Raw, InDataPtr);
               P_NATIVE_END;
               bSuccess = true;
            }
            else
            {
               FBlueprintExceptionInfo ExceptionInfo(
                  EBlueprintExceptionType::AccessViolation,
                  NSLOCTEXT("Rpai", "IncompatibleStructType", "Incompatible output parameter; the memory's type is not the same as the return type.")
               );
               FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ExceptionInfo);
            }
         }
         else
         {
            FBlueprintExceptionInfo ExceptionInfo(
               EBlueprintExceptionType::AccessViolation,
               NSLOCTEXT("Rpai", "MissingType", "Failed to resolve the output parameter for ReadAsStruct.")
            );
            FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ExceptionInfo);
         }
      }
      *(bool*)RESULT_PARAM = bSuccess;
   }
};
