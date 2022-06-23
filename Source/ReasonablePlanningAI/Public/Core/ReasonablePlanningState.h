// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/ReasonablePlanningTypes.h"
#include "ReasonablePlanningState.generated.h"

/**
 * A generic copyable state used for tracking actions and planning action sets
 */
UCLASS(BlueprintType, Blueprintable, Abstract, EditInlineNew, DefaultToInstanced)
class REASONABLEPLANNINGAI_API UReasonablePlanningState : public UObject
{
	GENERATED_BODY()

public:

    /**
     * Sets the boolean value defined with the given name. Implementation determines if values can be created (new) or only ever set based on a
     * previous definition (acting more like a class/object with defined properties)
     */
	UFUNCTION(BlueprintCallable, Category = "RPAI")
	virtual void SetBool(FName ValueName, bool Value) PURE_VIRTUAL(UReasonablePlanningState::SetBool, );

    /**
     * Sets the class value defined with the given name. Implementation determines if values can be created (new) or only ever set based on a
     * previous definition (acting more like a class/object with defined properties)
     */
	UFUNCTION(BlueprintCallable, Category = "RPAI")
	virtual void SetClassValue(FName ValueName, UClass* Value) PURE_VIRTUAL(UReasonablePlanningState::SetClassValue, );

    /**
     * Sets the enum value defined with the given name with the defined enum type. Implementation determines if values can be created (new) or only ever set based on a
     * previous definition (acting more like a class/object with defined properties)
     */
	UFUNCTION(BlueprintCallable, Category = "RPAI")
	virtual void SetEnum(FName ValueName, UEnum* Type, uint8 Value) PURE_VIRTUAL(UReasonablePlanningState::SetEnum, );

    /**
     * Sets the float value defined with the given name. Implementation determines if values can be created (new) or only ever set based on a
     * previous definition (acting more like a class/object with defined properties)
     */
	UFUNCTION(BlueprintCallable, Category = "RPAI")
	virtual void SetFloat(FName ValueName, float Value) PURE_VIRTUAL(UReasonablePlanningState::SetFloat, );

    /**
     * Sets the int value defined with the given name. Implementation determines if values can be created (new) or only ever set based on a
     * previous definition (acting more like a class/object with defined properties)
     */
	UFUNCTION(BlueprintCallable, Category = "RPAI")
	virtual void SetInt(FName ValueName, int32 Value) PURE_VIRTUAL(UReasonablePlanningState::SetInt, );

    /**
     * Sets the name value defined with the given name. Implementation determines if values can be created (new) or only ever set based on a
     * previous definition (acting more like a class/object with defined properties)
     */
	UFUNCTION(BlueprintCallable, Category = "RPAI")
	virtual void SetNameValue(FName ValueName, FName Value) PURE_VIRTUAL(UReasonablePlanningState::SetName, );

    /**
     * Sets the object value defined with the given name. Implementation determines if values can be created (new) or only ever set based on a
     * previous definition (acting more like a class/object with defined properties)
     */
	UFUNCTION(BlueprintCallable, Category = "RPAI")
	virtual void SetObject(FName ValueName, UObject* Value) PURE_VIRTUAL(UReasonablePlanningState::SetObject, );

    /**
     * Sets the rotator value defined with the given name. Implementation determines if values can be created (new) or only ever set based on a
     * previous definition (acting more like a class/object with defined properties)
     */
	UFUNCTION(BlueprintCallable, Category = "RPAI")
	virtual void SetRotator(FName ValueName, FRotator Value) PURE_VIRTUAL(UReasonablePlanningState::SetRotator, );

    /**
     * Sets the string value defined with the given name. Implementation determines if values can be created (new) or only ever set based on a
     * previous definition (acting more like a class/object with defined properties)
     */
	UFUNCTION(BlueprintCallable, Category = "RPAI")
	virtual void SetString(FName ValueName, FString Value) PURE_VIRTUAL(UReasonablePlanningState::SetString, );

    /**
     * Sets the vector value defined with the given name. Implementation determines if values can be created (new) or only ever set based on a
     * previous definition (acting more like a class/object with defined properties)
     */
	UFUNCTION(BlueprintCallable, Category = "RPAI")
	virtual void SetVector(FName ValueName, FVector Value) PURE_VIRTUAL(UReasonablePlanningState::SetVector, );

    /**
     * Gets the boolean value defined with the given name.
     *
     * @Param ValueName the key name of the value in the state to get
     * @Param OutValue reference to the value that will serve as storage for the copied value found in the state
     * @Returns true if the value was found and OutValue is valid for usage, false otherwise
     */
	UFUNCTION(BlueprintPure, Category = "RPAI")
	virtual bool GetBool(FName ValueName, bool& OutValue) const PURE_VIRTUAL(UReasonablePlanningState::GetBool, return false;);

    /**
     * Gets the class value defined with the given name.
     *
     * @Param ValueName the key name of the value in the state to get
     * @Param OutValue reference to the value that will serve as storage for the copied value found in the state
     * @Returns true if the value was found and OutValue is valid for usage, false otherwise
     */
	UFUNCTION(BlueprintPure, Category = "RPAI")
	virtual bool GetClassValue(FName ValueName, UClass*& OutValue) const PURE_VIRTUAL(UReasonablePlanningState::GetClassValue, return false;);

    /**
     * Gets the enum value defined with the given name and type
     *
     * @Param ValueName the key name of the value in the state to get
     * @Param OutValue reference to the value that will serve as storage for the copied value found in the state
     * @Returns true if the value was found and OutValue is valid for usage, false otherwise
     */
	UFUNCTION(BlueprintPure, Category = "RPAI")
	virtual bool GetEnum(FName ValueName, UEnum* Type, uint8& OutValue) const PURE_VIRTUAL(UReasonablePlanningState::GetEnum, return false;);

    /**
     * Gets the float value defined with the given name.
     *
     * @Param ValueName the key name of the value in the state to get
     * @Param OutValue reference to the value that will serve as storage for the copied value found in the state
     * @Returns true if the value was found and OutValue is valid for usage, false otherwise
     */
	UFUNCTION(BlueprintPure, Category = "RPAI")
	virtual bool GetFloat(FName ValueName, float& OutValue) const PURE_VIRTUAL(UReasonablePlanningState::GetFloat, return false;);

    /**
     * Gets the int value defined with the given name.
     *
     * @Param ValueName the key name of the value in the state to get
     * @Param OutValue reference to the value that will serve as storage for the copied value found in the state
     * @Returns true if the value was found and OutValue is valid for usage, false otherwise
     */
	UFUNCTION(BlueprintPure, Category = "RPAI")
	virtual bool GetInt(FName ValueName, int32& OutValue) const PURE_VIRTUAL(UReasonablePlanningState::GetInt, return false;);

    /**
     * Gets the name value defined with the given name.
     *
     * @Param ValueName the key name of the value in the state to get
     * @Param OutValue reference to the value that will serve as storage for the copied value found in the state
     * @Returns true if the value was found and OutValue is valid for usage, false otherwise
     */
	UFUNCTION(BlueprintPure, Category = "RPAI")
	virtual bool GetNameValue(FName ValueName, FName& OutValue) const PURE_VIRTUAL(UReasonablePlanningState::GetNameValue, return false;);

    /**
     * Gets the object value defined with the given name.
     *
     * @Param ValueName the key name of the value in the state to get
     * @Param OutValue reference to the value that will serve as storage for the copied value found in the state
     * @Returns true if the value was found and OutValue is valid for usage, false otherwise
     */
	UFUNCTION(BlueprintPure, Category = "RPAI")
	virtual bool GetObject(FName ValueName, UObject*& OutValue) const PURE_VIRTUAL(UReasonablePlanningState::GetObject, return false;);

    /**
     * Gets the rotator value defined with the given name.
     *
     * @Param ValueName the key name of the value in the state to get
     * @Param OutValue reference to the value that will serve as storage for the copied value found in the state
     * @Returns true if the value was found and OutValue is valid for usage, false otherwise
     */
	UFUNCTION(BlueprintPure, Category = "RPAI")
	virtual bool GetRotator(FName ValueName, FRotator& OutValue) const PURE_VIRTUAL(UReasonablePlanningState::GetRotator, return false;);

    /**
     * Gets the string value defined with the given name.
     *
     * @Param ValueName the key name of the value in the state to get
     * @Param OutValue reference to the value that will serve as storage for the copied value found in the state
     * @Returns true if the value was found and OutValue is valid for usage, false otherwise
     */
	UFUNCTION(BlueprintPure, Category = "RPAI")
	virtual bool GetString(FName ValueName, FString& OutValue) const PURE_VIRTUAL(UReasonablePlanningState::GetString, return false;);

    /**
     * Gets the vector value defined with the given name.
     *
     * @Param ValueName the key name of the value in the state to get
     * @Param OutValue reference to the value that will serve as storage for the copied value found in the state
     * @Returns true if the value was found and OutValue is valid for usage, false otherwise
     */
	UFUNCTION(BlueprintPure, Category = "RPAI")
	virtual bool GetVector(FName ValueName, FVector& OutValue) const PURE_VIRTUAL(UReasonablePlanningState::GetVector, return false;);

	UFUNCTION(BlueprintPure, Category = "RPAI")
	virtual bool HasBool(FName ValueName) const PURE_VIRTUAL(UReasonablePlanningState::HasBool, return false;);

	UFUNCTION(BlueprintPure, Category = "RPAI")
	virtual bool HasClass(FName ValueName) const PURE_VIRTUAL(UReasonablePlanningState::HasClass, return false;);

	UFUNCTION(BlueprintPure, Category = "RPAI")
	virtual bool HasEnum(FName ValueName) const PURE_VIRTUAL(UReasonablePlanningState::HasEnum, return false;);

	UFUNCTION(BlueprintPure, Category = "RPAI")
	virtual bool HasFloat(FName ValueName) const PURE_VIRTUAL(UReasonablePlanningState::HasFloat, return false;);

	UFUNCTION(BlueprintPure, Category = "RPAI")
	virtual bool HasInt(FName ValueName) const PURE_VIRTUAL(UReasonablePlanningState::HasInt, return false;);

	UFUNCTION(BlueprintPure, Category = "RPAI")
	virtual bool HasName(FName ValueName) const PURE_VIRTUAL(UReasonablePlanningState::HasName, return false;);

	UFUNCTION(BlueprintPure, Category = "RPAI")
	virtual bool HasObject(FName ValueName) const PURE_VIRTUAL(UReasonablePlanningState::HasObject, return false;);

	UFUNCTION(BlueprintPure, Category = "RPAI")
	virtual bool HasRotator(FName ValueName) const PURE_VIRTUAL(UReasonablePlanningState::HasRotator, return false;);

	UFUNCTION(BlueprintPure, Category = "RPAI")
	virtual bool HasString(FName ValueName) const PURE_VIRTUAL(UReasonablePlanningState::HasString, return false;);

	UFUNCTION(BlueprintPure, Category = "RPAI")
	virtual bool HasVector(FName ValueName) const PURE_VIRTUAL(UReasonablePlanningState::HasVector, return false;);

	UFUNCTION(BlueprintPure, Category = "RPAI")
	virtual bool HasValueWithName(FName ValueName) const PURE_VIRTUAL(UReasonablePlanningState::HasValueWithName, return false;);

	UFUNCTION(BlueprintPure, Category = "RPAI")
	virtual EStatePropertyType GetTypeFromName(FName ValueName) const PURE_VIRTUAL(UReasonablePlanningState::GetTypeFromName, return EStatePropertyType::Invalid;);

	UFUNCTION(BlueprintPure, Category = "RPAI")
	bool HasReferencedState(const FStateKeyValueReference & StatePropertyReference) const;

	UFUNCTION(BlueprintCallable, Category = "RPAI")
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
	UFUNCTION(BlueprintCallable, Category = "RPAI")
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
