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

    /**
    * Check to verify the state has a boolean with the given name
    *
    * @Param ValueName the key name of the value in the state to verify
    * @Returns true if the value was with the given name of this type was found
    */
	UFUNCTION(BlueprintPure, Category = "RPAI")
	virtual bool HasBool(FName ValueName) const PURE_VIRTUAL(UReasonablePlanningState::HasBool, return false;);

    /**
    * Check to verify the state has a class with the given name
    *
    * @Param ValueName the key name of the value in the state to verify
    * @Returns true if the value was with the given name of this type was found
    */
	UFUNCTION(BlueprintPure, Category = "RPAI")
	virtual bool HasClass(FName ValueName) const PURE_VIRTUAL(UReasonablePlanningState::HasClass, return false;);

    /**
    * Check to verify the state has a enum with the given name
    *
    * @Param ValueName the key name of the value in the state to verify
    * @Returns true if the value was with the given name of this type was found
    */
	UFUNCTION(BlueprintPure, Category = "RPAI")
	virtual bool HasEnum(FName ValueName) const PURE_VIRTUAL(UReasonablePlanningState::HasEnum, return false;);

    /**
    * Check to verify the state has a float with the given name
    *
    * @Param ValueName the key name of the value in the state to verify
    * @Returns true if the value was with the given name of this type was found
    */
	UFUNCTION(BlueprintPure, Category = "RPAI")
	virtual bool HasFloat(FName ValueName) const PURE_VIRTUAL(UReasonablePlanningState::HasFloat, return false;);

    /**
    * Check to verify the state has a integer with the given name
    *
    * @Param ValueName the key name of the value in the state to verify
    * @Returns true if the value was with the given name of this type was found
    */
	UFUNCTION(BlueprintPure, Category = "RPAI")
	virtual bool HasInt(FName ValueName) const PURE_VIRTUAL(UReasonablePlanningState::HasInt, return false;);

    /**
    * Check to verify the state has a name with the given name
    *
    * @Param ValueName the key name of the value in the state to verify
    * @Returns true if the value was with the given name of this type was found
    */
	UFUNCTION(BlueprintPure, Category = "RPAI")
	virtual bool HasName(FName ValueName) const PURE_VIRTUAL(UReasonablePlanningState::HasName, return false;);

    /**
    * Check to verify the state has a object with the given name
    *
    * @Param ValueName the key name of the value in the state to verify
    * @Returns true if the value was with the given name of this type was found
    */
	UFUNCTION(BlueprintPure, Category = "RPAI")
	virtual bool HasObject(FName ValueName) const PURE_VIRTUAL(UReasonablePlanningState::HasObject, return false;);

    /**
    * Check to verify the state has a rotator with the given name
    *
    * @Param ValueName the key name of the value in the state to verify
    * @Returns true if the value was with the given name of this type was found
    */
	UFUNCTION(BlueprintPure, Category = "RPAI")
	virtual bool HasRotator(FName ValueName) const PURE_VIRTUAL(UReasonablePlanningState::HasRotator, return false;);

    /**
    * Check to verify the state has a string with the given name
    *
    * @Param ValueName the key name of the value in the state to verify
    * @Returns true if the value was with the given name of this type was found
    */
	UFUNCTION(BlueprintPure, Category = "RPAI")
	virtual bool HasString(FName ValueName) const PURE_VIRTUAL(UReasonablePlanningState::HasString, return false;);

    /**
    * Check to verify the state has a vector with the given name
    *
    * @Param ValueName the key name of the value in the state to verify
    * @Returns true if the value was with the given name of this type was found
    */
	UFUNCTION(BlueprintPure, Category = "RPAI")
	virtual bool HasVector(FName ValueName) const PURE_VIRTUAL(UReasonablePlanningState::HasVector, return false;);

    /**
    * Check to verify the state has a any type of value with the given name
    *
    * @Param ValueName the key name of the value in the state to verify
    * @Returns true if the value was with the given name of this type was found
    */
	UFUNCTION(BlueprintPure, Category = "RPAI")
	virtual bool HasValueWithName(FName ValueName) const PURE_VIRTUAL(UReasonablePlanningState::HasValueWithName, return false;);

    /**
    * Check to verify the state has a boolean with the given name
    *
    * @Param ValueName the key name of the value in the state to verify
    * @Returns true if the value was with the given name of this type was found
    */
	UFUNCTION(BlueprintPure, Category = "RPAI")
	virtual EStatePropertyType GetTypeFromName(FName ValueName) const PURE_VIRTUAL(UReasonablePlanningState::GetTypeFromName, return EStatePropertyType::Invalid;);

    /**
    * Check to verify the state has a boolean with the given name
    *
    * @Param ValueName the key name of the value in the state to verify
    * @Returns true if the value was with the given name of this type was found
    */
	UFUNCTION(BlueprintPure, Category = "RPAI")
	bool HasReferencedState(const FStateKeyValueReference & StatePropertyReference) const;

    /**
    * Takes all state properties needed for planning and prediction and copies them to the given state.
    *
    * @Param OtherState the state to copy values to
    */
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

    // C++ Template Helpers

    template<typename T>
    FORCEINLINE bool HasValueOfType(FName ValueName) const { return false; }

    template<typename T>
    FORCEINLINE bool GetValueOfType(FName ValueName, T& OutValue) const { return false; }

    template<typename T>
    FORCEINLINE void SetValueOfType(FName ValueName, T NewValue) { }


    // Helper Specializations

    // Has

    template<>
    FORCEINLINE bool HasValueOfType<float>(FName ValueName) const { return HasFloat(ValueName); }

    template<>
    FORCEINLINE bool HasValueOfType<bool>(FName ValueName) const { return HasBool(ValueName); }

    template<>
    FORCEINLINE bool HasValueOfType<int32>(FName ValueName) const { return HasInt(ValueName); }

    template<>
    FORCEINLINE bool HasValueOfType<UClass>(FName ValueName) const { return HasClass(ValueName); }

    template<>
    FORCEINLINE bool HasValueOfType<FName>(FName ValueName) const { return HasName(ValueName); }

    template<>
    FORCEINLINE bool HasValueOfType<FString>(FName ValueName) const { return HasString(ValueName); }

    template<>
    FORCEINLINE bool HasValueOfType<FVector>(FName ValueName) const { return HasVector(ValueName); }

    template<>
    FORCEINLINE bool HasValueOfType<FRotator>(FName ValueName) const { return HasRotator(ValueName); }

    template<>
    FORCEINLINE bool HasValueOfType<UObject>(FName ValueName) const { return HasObject(ValueName); }

    // Get

    template<>
    FORCEINLINE bool GetValueOfType(FName ValueName, float& OutValue) const { return GetFloat(ValueName, OutValue); }

    template<>
    FORCEINLINE bool GetValueOfType(FName ValueName, bool& OutValue) const { return GetBool(ValueName, OutValue); }

    template<>
    FORCEINLINE bool GetValueOfType(FName ValueName, int32& OutValue) const { return GetInt(ValueName, OutValue); }

    template<>
    FORCEINLINE bool GetValueOfType(FName ValueName, UClass*& OutValue) const { return GetClassValue(ValueName, OutValue); }

    template<>
    FORCEINLINE bool GetValueOfType(FName ValueName, FName& OutValue) const { return GetNameValue(ValueName, OutValue); }

    template<>
    FORCEINLINE bool GetValueOfType(FName ValueName, FString& OutValue) const { return GetString(ValueName, OutValue); }

    template<>
    FORCEINLINE bool GetValueOfType(FName ValueName, FVector& OutValue) const { return GetVector(ValueName, OutValue); }

    template<>
    FORCEINLINE bool GetValueOfType(FName ValueName, FRotator& OutValue) const { return GetRotator(ValueName, OutValue); }

    template<>
    FORCEINLINE bool GetValueOfType(FName ValueName, UObject*& OutValue) const { return GetObject(ValueName, OutValue); }

    // Set

    template<>
    FORCEINLINE void SetValueOfType(FName ValueName, float NewValue) { SetFloat(ValueName, NewValue); }

    template<>
    FORCEINLINE void SetValueOfType(FName ValueName, bool NewValue) { SetBool(ValueName, NewValue); }

    template<>
    FORCEINLINE void SetValueOfType(FName ValueName, int32 NewValue) { SetInt(ValueName, NewValue); }

    template<>
    FORCEINLINE void SetValueOfType(FName ValueName, UClass* NewValue) { SetClassValue(ValueName, NewValue); }

    template<>
    FORCEINLINE void SetValueOfType(FName ValueName, FName NewValue) { SetNameValue(ValueName, NewValue); }

    template<>
    FORCEINLINE void SetValueOfType(FName ValueName, FString NewValue) { SetString(ValueName, NewValue); }

    template<>
    FORCEINLINE void SetValueOfType(FName ValueName, FVector NewValue) { SetVector(ValueName, NewValue); }

    template<>
    FORCEINLINE void SetValueOfType(FName ValueName, FRotator NewValue) { SetRotator(ValueName, NewValue); }

    template<>
    FORCEINLINE void SetValueOfType(FName ValueName, UObject* NewValue) { SetObject(ValueName, NewValue); }
};
