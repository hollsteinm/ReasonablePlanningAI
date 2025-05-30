// Radaway Software LLC. 2025. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "ComposerBehaviorFactory.generated.h"

/**
 * 
 */
UCLASS()
class REASONABLEPLANNINGAIEDITOR_API UComposerBehaviorFactory : public UFactory
{
	GENERATED_BODY()
public:
    UComposerBehaviorFactory();

    UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn);
};
