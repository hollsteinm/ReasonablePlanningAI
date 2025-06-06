// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.


#include "ComposerBehaviorFactory.h"
#include "Composer/RpaiComposerBehavior.h"

UComposerBehaviorFactory::UComposerBehaviorFactory()
{
    SupportedClass = URpaiComposerBehavior::StaticClass();
    bCreateNew = true;
}

UObject* UComposerBehaviorFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
    return NewObject<URpaiComposerBehavior>(InParent, Class, Name, Flags, Context);
}
