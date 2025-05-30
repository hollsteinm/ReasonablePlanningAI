// Radaway Software LLC. 2025. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/RpaiComposerWeightBase.h"
#include "RpaiWeight_Select.generated.h"

class URpaiComposerStateQuery;

USTRUCT(BlueprintType)
struct FWeightSelectStateQueryPair
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, Instanced, Category = Rpai)
	URpaiComposerStateQuery* SelectionQuery;

	UPROPERTY(EditAnywhere, Instanced, Category=Rpai)
	URpaiComposerWeightBase* SelectionWeight;
};

/**
 * Given a list of StateQuery and Weight Pairs, execute the Weight algorithm associated with the first passing StateQuery. The structure of your
 * array of Selections should be thought of as a structure of if and else if statements. The default is the else statement.
 */
UCLASS()
class REASONABLEPLANNINGAI_API URpaiWeight_Select : public URpaiComposerWeightBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = Rpai)
	URpaiComposerWeightBase* Default;

	UPROPERTY(EditAnywhere, Category = Rpai)
	TArray<FWeightSelectStateQueryPair> Selections;

	virtual float ReceiveExecutionWeight_Implementation(const URpaiState* GivenState) const override;

public:
	FORCEINLINE void SetSelections(TArray<FWeightSelectStateQueryPair>&& NewSelections) { Selections = NewSelections; }

	FORCEINLINE void SetDefault(URpaiComposerWeightBase* NewDefault) { Default = NewDefault; }
};
