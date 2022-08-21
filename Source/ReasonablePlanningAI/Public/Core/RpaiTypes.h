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
