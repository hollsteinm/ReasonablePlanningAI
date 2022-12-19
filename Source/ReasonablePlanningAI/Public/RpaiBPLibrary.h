// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Core/RpaiTypes.h"
#include "RpaiBPLibrary.generated.h"

class URpaiState;

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
};
