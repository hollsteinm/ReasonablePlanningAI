// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ReasonablePlanningTypes.h"
#include "ReasonablePlanningState.generated.h"

/**
 * A generic copyable state used for tracking actions and planning action sets
 */
UCLASS(BlueprintType, Blueprintable, Abstract, EditInlineNew, DefaultToInstanced)
class REASONABLEPLANNINGAI_API UReasonablePlanningState : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category="ReasonablePlanningAI")
	virtual void SetBool(FName ValueName, bool Value) PURE_VIRTUAL(UReasonablePlanningState::SetBool, );

	UFUNCTION(BlueprintCallable, Category = "ReasonablePlanningAI")
	virtual void SetClassValue(FName ValueName, UClass* Value) PURE_VIRTUAL(UReasonablePlanningState::SetClassValue, );

	UFUNCTION(BlueprintCallable, Category = "ReasonablePlanningAI")
	virtual void SetEnum(FName ValueName, UEnum* Type, uint8 Value) PURE_VIRTUAL(UReasonablePlanningState::SetEnum, );

	UFUNCTION(BlueprintCallable, Category = "ReasonablePlanningAI")
	virtual void SetFloat(FName ValueName, float Value) PURE_VIRTUAL(UReasonablePlanningState::SetFloat, );

	UFUNCTION(BlueprintCallable, Category = "ReasonablePlanningAI")
	virtual void SetInt(FName ValueName, int32 Value) PURE_VIRTUAL(UReasonablePlanningState::SetInt, );

	UFUNCTION(BlueprintCallable, Category = "ReasonablePlanningAI")
	virtual void SetNameValue(FName ValueName, FName Value) PURE_VIRTUAL(UReasonablePlanningState::SetName, );

	UFUNCTION(BlueprintCallable, Category = "ReasonablePlanningAI")
	virtual void SetObject(FName ValueName, UObject* Value) PURE_VIRTUAL(UReasonablePlanningState::SetObject, );

	UFUNCTION(BlueprintCallable, Category = "ReasonablePlanningAI")
	virtual void SetRotator(FName ValueName, FRotator Value) PURE_VIRTUAL(UReasonablePlanningState::SetRotator, );

	UFUNCTION(BlueprintCallable, Category = "ReasonablePlanningAI")
	virtual void SetString(FName ValueName, FString Value) PURE_VIRTUAL(UReasonablePlanningState::SetString, );

	UFUNCTION(BlueprintCallable, Category = "ReasonablePlanningAI")
	virtual void SetVector(FName ValueName, FVector Value) PURE_VIRTUAL(UReasonablePlanningState::SetVector, );

	UFUNCTION(BlueprintPure, Category = "ReasonablePlanningAI")
	virtual bool GetBool(FName ValueName, bool& OutValue) const PURE_VIRTUAL(UReasonablePlanningState::GetBool, return false;);

	UFUNCTION(BlueprintPure, Category = "ReasonablePlanningAI")
	virtual bool GetClassValue(FName ValueName, UClass*& OutValue) const PURE_VIRTUAL(UReasonablePlanningState::GetClassValue, return false;);

	UFUNCTION(BlueprintPure, Category = "ReasonablePlanningAI")
	virtual bool GetEnum(FName ValueName, UEnum* Type, uint8& OutValue) const PURE_VIRTUAL(UReasonablePlanningState::GetEnum, return false;);

	UFUNCTION(BlueprintPure, Category = "ReasonablePlanningAI")
	virtual bool GetFloat(FName ValueName, float& OutValue) const PURE_VIRTUAL(UReasonablePlanningState::GetFloat, return false;);

	UFUNCTION(BlueprintPure, Category = "ReasonablePlanningAI")
	virtual bool GetInt(FName ValueName, int32& OutValue) const PURE_VIRTUAL(UReasonablePlanningState::GetInt, return false;);

	UFUNCTION(BlueprintPure, Category = "ReasonablePlanningAI")
	virtual bool GetNameValue(FName ValueName, FName& OutValue) const PURE_VIRTUAL(UReasonablePlanningState::GetName, return false;);

	UFUNCTION(BlueprintPure, Category = "ReasonablePlanningAI")
	virtual bool GetObject(FName ValueName, UObject*& OutValue) const PURE_VIRTUAL(UReasonablePlanningState::GetObject, return false;);

	UFUNCTION(BlueprintPure, Category = "ReasonablePlanningAI")
	virtual bool GetRotator(FName ValueName, FRotator& OutValue) const PURE_VIRTUAL(UReasonablePlanningState::GetRotator, return false;);

	UFUNCTION(BlueprintPure, Category = "ReasonablePlanningAI")
	virtual bool GetString(FName ValueName, FString& OutValue) const PURE_VIRTUAL(UReasonablePlanningState::GetString, return false;);

	UFUNCTION(BlueprintPure, Category = "ReasonablePlanningAI")
	virtual bool GetVector(FName ValueName, FVector& OutValue) const PURE_VIRTUAL(UReasonablePlanningState::GetVector, return false;);

	UFUNCTION(BlueprintPure, Category = "ReasonablePlanningAI")
	virtual bool HasBool(FName ValueName) const PURE_VIRTUAL(UReasonablePlanningState::HasBool, return false;);

	UFUNCTION(BlueprintPure, Category = "ReasonablePlanningAI")
	virtual bool HasClass(FName ValueName) const PURE_VIRTUAL(UReasonablePlanningState::HasClass, return false;);

	UFUNCTION(BlueprintPure, Category = "ReasonablePlanningAI")
	virtual bool HasEnum(FName ValueName) const PURE_VIRTUAL(UReasonablePlanningState::HasEnum, return false;);

	UFUNCTION(BlueprintPure, Category = "ReasonablePlanningAI")
	virtual bool HasFloat(FName ValueName) const PURE_VIRTUAL(UReasonablePlanningState::HasFloat, return false;);

	UFUNCTION(BlueprintPure, Category = "ReasonablePlanningAI")
	virtual bool HasInt(FName ValueName) const PURE_VIRTUAL(UReasonablePlanningState::HasInt, return false;);

	UFUNCTION(BlueprintPure, Category = "ReasonablePlanningAI")
	virtual bool HasName(FName ValueName) const PURE_VIRTUAL(UReasonablePlanningState::HasName, return false;);

	UFUNCTION(BlueprintPure, Category = "ReasonablePlanningAI")
	virtual bool HasObject(FName ValueName) const PURE_VIRTUAL(UReasonablePlanningState::HasObject, return false;);

	UFUNCTION(BlueprintPure, Category = "ReasonablePlanningAI")
	virtual bool HasRotator(FName ValueName) const PURE_VIRTUAL(UReasonablePlanningState::HasRotator, return false;);

	UFUNCTION(BlueprintPure, Category = "ReasonablePlanningAI")
	virtual bool HasString(FName ValueName) const PURE_VIRTUAL(UReasonablePlanningState::HasString, return false;);

	UFUNCTION(BlueprintPure, Category = "ReasonablePlanningAI")
	virtual bool HasVector(FName ValueName) const PURE_VIRTUAL(UReasonablePlanningState::HasVector, return false;);

	UFUNCTION(BlueprintPure, Category = "ReasonablePlanningAI")
	virtual bool HasValueWithName(FName ValueName) const PURE_VIRTUAL(UReasonablePlanningState::HasValueWithName, return false;);

	UFUNCTION(BlueprintPure, Category = "ReasonablePlanningAI")
	virtual EStatePropertyType GetTypeFromName(FName ValueName) const PURE_VIRTUAL(UReasonablePlanningState::GetTypeFromName, return EStatePropertyType::Invalid;);

	UFUNCTION(BlueprintPure, Category = "ReasonablePlanningAI")
	bool HasReferencedState(const FStateKeyValueReference & StatePropertyReference) const;

	UFUNCTION(BlueprintCallable, Category = "ReasonablePlanningAI")
	virtual void CopyStateForPredictionTo(UReasonablePlanningState* OtherState) const PURE_VIRTUAL(UReasonablePlanningState::CopyStateForPreditionTo,);

	/**
	* Query if the given resource is locked by any object
	*/
	UFUNCTION(BlueprintPure, Category = "ResonablePlanningAI")
	virtual bool IsResourceLocked(FName ExactName) const PURE_VIRTUAL(UReasonablePlanningState::IsResourceLocked, return false;);

	/**
	* Query if the given resource is not locked by any object
	*/
	UFUNCTION(BlueprintPure, Category = "ResonablePlanningAI")
	virtual bool IsResourceFree(FName ExactName) const PURE_VIRTUAL(UReasonablePlanningState::IsResourceFree, return false;);

	/**
	* Query if the given path prefix of a collection of resources are locked
	*/
	UFUNCTION(BlueprintCallable, Category = "ReasonablePlanningAI")
	virtual bool DoesHeirarchyHaveAnyLocks(FName PathPrefix) const PURE_VIRTUAL(UReasonablePlanningState::DoesHeirarchyHaveAnyLocks, return false;);

	/**
	* Locks the resource to the given LockedBy as the original Locker. Returns
	* true if the resource is successfully locked with given valid params. Will
	* Lazily create a resource if it does not exist.
	*/
	UFUNCTION(BlueprintCallable, Category = "ResonablePlanningAI")
	virtual bool LockResource(FName ExactName, UObject* OptionalLockingObject = nullptr) PURE_VIRTUAL(UReasonablePlanningState::LockResource, return false;);

	/**
	* Unlocks the resource if the give LockedBy is the original Locker. Returns
	* true if the resource is successfully unlocked with given valid params.
	*/
	UFUNCTION(BlueprintCallable, Category = "ResonablePlanningAI")
	virtual bool UnlockResource(FName ExactName, UObject* OptionalLockingObject = nullptr) PURE_VIRTUAL(UReasonablePlanningState::UnlockResource, return false;);
};
