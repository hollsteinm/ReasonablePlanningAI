// Copyright Troll Purse. All Rights Reserved.

#include "ReasonablePlanningAITestSuite.h"

#define LOCTEXT_NAMESPACE "ResonablePlanningAITestSuite"

struct FReasonablePlanningAITestSuiteModule : public IReasonablePlanningAITestSuiteModule
{
};

IMPLEMENT_MODULE(FReasonablePlanningAITestSuiteModule, ReasonablePlanningAITestSuite)

#undef LOCTEXT_NAMESPACE