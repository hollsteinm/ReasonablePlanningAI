// Radaway Software LLC. 2025. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/RpaiTypes.h"
#include "Composer/RpaiComposerActionTaskBase.h"
#include "GameplayTaskOwnerInterface.h"
#include "RpaiComposerActionTask.generated.h"

/**
 * Data driven action task with data values pulled from the state interface. Supports Gameplay Tasks
 */
UCLASS(Blueprintable, BlueprintType, Abstract)
class REASONABLEPLANNINGAI_API URpaiComposerActionTask : public URpaiComposerActionTaskBase
{
   GENERATED_BODY()

protected:
   UPROPERTY(EditAnywhere, Category = "Rpai")
   FStateKeyValueReference ActionTaskStateKeyValueReference;

public:
   URpaiComposerActionTask();

   FORCEINLINE void SetStateKeyValueReference(const FStateKeyValueReference& Reference) { ActionTaskStateKeyValueReference = Reference; }
};
