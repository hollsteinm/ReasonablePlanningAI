// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ReasonablePlanningTypes.generated.h"

DECLARE_STATS_GROUP(TEXT("RPAI"), STATGROUP_RPAI, STATCAT_Advanced);

DECLARE_LOG_CATEGORY_EXTERN(LogRPAI, Log, All);

/**
* Represents basic types supported by the ReasonablePlanningState
*/
UENUM(BlueprintType, Category = "RPAI")
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
USTRUCT(BlueprintType, Category = "RPAI")
struct REASONABLEPLANNINGAI_API FStateKeyValueReference
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RPAI")
	FName StateKeyName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RPAI")
	EStatePropertyType ExpectedValueType;
};
