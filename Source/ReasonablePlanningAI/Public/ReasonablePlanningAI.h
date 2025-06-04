// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FReasonablePlanningAIModule : public IModuleInterface
{
public:

   /** IModuleInterface implementation */
   virtual void StartupModule() override;
   virtual void ShutdownModule() override;
};
