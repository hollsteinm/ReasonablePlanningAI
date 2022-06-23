// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ReasonablePlanningResourceCollection.generated.h"

/**
 * Helper class to lock resources related to state
 */
UCLASS(BlueprintType, Blueprintable)
class REASONABLEPLANNINGAI_API UReasonablePlanningResourceCollection : public UObject
{
	GENERATED_BODY()

public:
	/**
	* Query if the given resource is locked by any object
	*/
	UFUNCTION(BlueprintCallable, Category = "RPAI")
	bool IsResourceLocked(FName ExactName) const;

	/**
	* Query if the given resource is not locked by any object
	*/
	UFUNCTION(BlueprintCallable, Category = "RPAI")
	bool IsResourceFree(FName ExactName) const;

	/**
	* Query if the given path prefix of a collection of resources are locked
	*/
	UFUNCTION(BlueprintCallable, Category = "RPAI")
	bool DoesHeirarchyHaveAnyLocks(FName PathPrefix) const;

	/**
	* Locks the resource to the given LockedBy as the original Locker. Returns
	* true if the resource is successfully locked with given valid params. Will
	* Lazily create a resource if it does not exist.
	*/
	UFUNCTION(BlueprintCallable, Category = "RPAI")
	bool LockResource(UObject* LockedBy, FName ExactName);

	/**
	* Unlocks the resource if the give LockedBy is the original Locker. Returns
	* true if the resource is successfully unlocked with given valid params.
	*/
	UFUNCTION(BlueprintCallable, Category = "RPAI")
	bool UnlockResource(UObject* LockedBy, FName ExactName);

private:
	UPROPERTY(SaveGame)
	TSet<FName> LockedResources;

	TMultiMap<UObject*, FName> LockedResourceByOwner;

	TMultiMap<FName, FName> ResourcePathReverseIndex;
};
