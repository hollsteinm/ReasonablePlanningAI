#include "ReasonablePlanningResourceCollection.h"

bool UReasonablePlanningResourceCollection::IsResourceLocked(FName ExactName) const
{
	return LockedResources.Contains(ExactName);
}

bool UReasonablePlanningResourceCollection::IsResourceFree(FName ExactName) const
{
	return !IsResourceLocked(ExactName);
}

bool UReasonablePlanningResourceCollection::DoesHeirarchyHaveAnyLocks(FName PathPrefix) const
{
	return ResourcePathReverseIndex.Contains(PathPrefix) || LockedResources.Contains(PathPrefix);
}

bool UReasonablePlanningResourceCollection::LockResource(UObject* LockedBy, FName ExactName)
{
	bool bIsNotNew = false;
	LockedResources.Add(ExactName, &bIsNotNew);
	if (bIsNotNew)
	{
		return false;
	}

	LockedResourceByOwner.EmplaceUnique(LockedBy, ExactName);

	FString Left;
	FString Right;
	FString Path;
	ExactName.ToString(Right);
	while (Right.Split(".", &Left, &Right))
	{
		Path.Append(Left);
		ResourcePathReverseIndex.EmplaceUnique(FName(Path), ExactName);
		Path.Append(".");
	}
	return true;
}

bool UReasonablePlanningResourceCollection::UnlockResource(UObject* LockedBy, FName ExactName)
{
	FName* FoundValue = LockedResourceByOwner.FindPair(LockedBy, ExactName);
	if (FoundValue == nullptr)
	{
		return false;
	}
	
	LockedResources.Remove(ExactName);
	LockedResourceByOwner.RemoveSingle(LockedBy, ExactName);
	FString Left;
	FString Right;
	FString Path;
	ExactName.ToString(Right);
	while (Right.Split(".", &Left, &Right))
	{
		Path.Append(Left);
		ResourcePathReverseIndex.RemoveSingle(FName(Path), ExactName);
		Path.Append(".");
	}
	return true;
}