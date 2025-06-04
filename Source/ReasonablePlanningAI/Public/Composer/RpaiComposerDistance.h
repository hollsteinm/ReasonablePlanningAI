// Radaway Software LLC. 2025. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/RpaiTypes.h"
#include "Composer/RpaiComposerDistanceBase.h"
#include "RpaiComposerDistance.generated.h"

/**
 * Algorithm class to determine the distance between two state values
 */
UCLASS(BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced)
class REASONABLEPLANNINGAI_API URpaiComposerDistance : public URpaiComposerDistanceBase
{
   GENERATED_BODY()

protected:

   /**
   * Left hand state key reference to use
   */
   UPROPERTY(EditAnywhere, Category = "Rpai", meta=(DisplayName="Left Hand Side State Reference Key"))
   FStateKeyValueReference LHS;

public:
   FORCEINLINE void SetLHS(FName Key, EStatePropertyType ExpectedType) { LHS.StateKeyName = Key; LHS.ExpectedValueType = ExpectedType; }
   
};
