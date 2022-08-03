// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/ReasonablePlanningWeightBase.h"
#include "Weight_Select.generated.h"

class UReasonablePlanningStateQuery;

USTRUCT(BlueprintType)
struct FWeightSelectStateQueryPair
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, Instanced, Category = RPAI)
	UReasonablePlanningStateQuery* SelectionQuery;

	UPROPERTY(EditAnywhere, Instanced, Category=RPAI)
	UReasonablePlanningWeightBase* SelectionWeight;
};

/**
 * Given a list of StateQuery and Weight Pairs, execute the Weight algorithm associated with the first passing StateQuery. The structure of your
 * array of Selections should be thought of as a structure of if and else if statements. The default is the else statement.
 */
UCLASS()
class REASONABLEPLANNINGAI_API UWeight_Select : public UReasonablePlanningWeightBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = RPAI)
	UReasonablePlanningWeightBase* Default;

	UPROPERTY(EditAnywhere, Category = RPAI)
	TArray<FWeightSelectStateQueryPair> Selections;

	virtual float ReceiveExecutionWeight_Implementation(const UReasonablePlanningState* GivenState) const override;

public:
	FORCEINLINE void SetSelections(TArray<FWeightSelectStateQueryPair>&& NewSelections) { Selections = NewSelections; }

	FORCEINLINE void SetDefault(UReasonablePlanningWeightBase* NewDefault) { Default = NewDefault; }
};
