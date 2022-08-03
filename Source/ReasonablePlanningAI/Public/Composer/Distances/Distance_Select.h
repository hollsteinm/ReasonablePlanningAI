// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/ReasonablePlanningDistanceBase.h"
#include "Distance_Select.generated.h"

class UReasonablePlanningStateQuery;

USTRUCT(BlueprintType)
struct FDistanceSelectStateQueryPair
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, Instanced, Category = RPAI)
	UReasonablePlanningStateQuery* SelectionQuery;

	UPROPERTY(EditAnywhere, Instanced, Category = RPAI)
	UReasonablePlanningDistanceBase* SelectionDistance;
};

/**
 * Given a list of StateQuery and Distance Pairs, execute the Distance algorithm associated with the first passing StateQuery. The structure of your
 * array of Selections should be thought of as a structure of if and else if statements. The default is the else statement.
 */
UCLASS()
class REASONABLEPLANNINGAI_API UDistance_Select : public UReasonablePlanningDistanceBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "RPAI")
	TArray<FDistanceSelectStateQueryPair> Selections;

	UPROPERTY(EditAnywhere, Category = "RPAI")
	UReasonablePlanningDistanceBase* Default;

	virtual float ReceiveCalculateDistance_Implementation(const UReasonablePlanningState* GivenState) const override;

public:
	FORCEINLINE void SetSelections(TArray<FDistanceSelectStateQueryPair>&& NewSelections) { Selections = NewSelections; }

	FORCEINLINE void SetDefault(UReasonablePlanningDistanceBase* NewDefault) { Default = NewDefault; }
};
