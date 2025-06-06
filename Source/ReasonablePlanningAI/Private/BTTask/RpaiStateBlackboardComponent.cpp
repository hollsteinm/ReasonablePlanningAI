// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.


#include "BTTask/RpaiStateBlackboardComponent.h"
#include "States/RpaiState_BlackboardComponentAdapter.h"

// Sets default values for this component's properties
URpaiStateBlackboardComponent::URpaiStateBlackboardComponent()
   : StateAdapter(nullptr)
{
   PrimaryComponentTick.bCanEverTick = false;
}
