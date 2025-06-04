// Radaway Software LLC. 2025. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "States/RpaiState_Map.h"
#include "RpaiState_BlackboardComponentAdapter.generated.h"

/**
 * Can read current state of a Blackboard to fill state values.
 */
UCLASS()
class REASONABLEPLANNINGAI_API URpaiState_BlackboardComponentAdapter : public URpaiState_Map
{
   GENERATED_BODY()
private:
   /**
   * Values configured here will be used to look up
   * and copy values from the blackboard based on the
   * configuration.
   */
   UPROPERTY(EditAnywhere, Category = "Rpai")
      TArray<FStateKeyValueReference> KeyLookupTable;

public:
   /**
   * Given a blackboard component, read the configured values from the blackboard for setting
   * the state's internal values.
   */
   uint32 ReadFromBlackboard(class UBlackboardComponent* SourceStateBlackboard);

   FORCEINLINE void SetKeyLookupTable(const TArray<FStateKeyValueReference>& NewKeyLookupTable) { KeyLookupTable = NewKeyLookupTable; }
};