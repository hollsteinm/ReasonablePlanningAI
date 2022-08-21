// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/RpaiComposerDistanceBase.h"
#include "RpaiDistance_Select.generated.h"

class URpaiComposerStateQuery;

USTRUCT(BlueprintType)
struct FDistanceSelectStateQueryPair
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, Instanced, Category = Rpai)
	URpaiComposerStateQuery* SelectionQuery;

	UPROPERTY(EditAnywhere, Instanced, Category = Rpai)
	URpaiComposerDistanceBase* SelectionDistance;
};

/**
 * Given a list of StateQuery and Distance Pairs, execute the Distance algorithm associated with the first passing StateQuery. The structure of your
 * array of Selections should be thought of as a structure of if and else if statements. The default is the else statement.
 */
UCLASS()
class REASONABLEPLANNINGAI_API URpaiDistance_Select : public URpaiComposerDistanceBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Rpai")
	TArray<FDistanceSelectStateQueryPair> Selections;

	UPROPERTY(EditAnywhere, Category = "Rpai")
	URpaiComposerDistanceBase* Default;

	virtual float ReceiveCalculateDistance_Implementation(const URpaiState* GivenState) const override;

public:
	FORCEINLINE void SetSelections(TArray<FDistanceSelectStateQueryPair>&& NewSelections) { Selections = NewSelections; }

	FORCEINLINE void SetDefault(URpaiComposerDistanceBase* NewDefault) { Default = NewDefault; }
};
