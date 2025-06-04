// Radaway Software LLC. 2025. All rights reserved.

#pragma once

#include "Engine/World.h"
#include "Subsystems/WorldSubsystem.h"
#include "RpaiSubsystem.generated.h"

class URpaiPlannerBase;

/**
 * World scoped execution of core components of Rpai.
 */
UCLASS()
class REASONABLEPLANNINGAI_API URpaiSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()
    
public:
    
    /**
     * Given a pointer to an instance to a planner, create a duplicate object if there exists no cached entry, the template is a class default, or it is not of the UWorld scoped to this World Subsystem.
     */
    UFUNCTION()
    const URpaiPlannerBase* DuplicateOrGetPlannerInstanceInWorldScope(const URpaiPlannerBase* Template);
    
    /**
     * Convenience getter to get the subsystem for the given UWorld
     */
    FORCEINLINE static URpaiSubsystem* GetCurrent(const UWorld* InWorld)
    {
        return UWorld::GetSubsystem<URpaiSubsystem>(InWorld);
    }
    
private:
    UPROPERTY(Transient)
    TMap<const URpaiPlannerBase*, const URpaiPlannerBase*> InstantiatedTemplates;
};
