// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/RpaiTypes.h"
#include "RpaiState.generated.h"

class AAIController;

/**
 * A generic copyable state used for tracking actions and planning action sets
 */
UCLASS(BlueprintType, Blueprintable, Abstract, EditInlineNew, DefaultToInstanced)
class REASONABLEPLANNINGAI_API URpaiState : public UObject
{
	GENERATED_BODY()

public:

    /**
    * In a system of N-M States to AIControllers where N exceeds M, it may be better to implement this function to support setting state rather than overriding the Brain Component method. This is what the default brain component uses to set state.
    **/
    virtual void SetStateFromController(const AAIController* FromController);

    /**
    * Determine if the internal state properties are equal to each other. Answer the question of, are we already in this state?
    */
    UFUNCTION(BlueprintPure, Category = "Rpai")
    virtual bool IsEqualTo(const URpaiState* OtherState) const;

    /**
     * Sets the boolean value defined with the given name. Implementation determines if values can be created (new) or only ever set based on a
     * previous definition (acting more like a class/object with defined properties)
     */
	UFUNCTION(BlueprintCallable, Category = "Rpai")
	virtual void SetBool(FName ValueName, bool Value) PURE_VIRTUAL(URpaiState::SetBool, );

    /**
     * Sets the class value defined with the given name. Implementation determines if values can be created (new) or only ever set based on a
     * previous definition (acting more like a class/object with defined properties)
     */
	UFUNCTION(BlueprintCallable, Category = "Rpai")
	virtual void SetClassValue(FName ValueName, UClass* Value) PURE_VIRTUAL(URpaiState::SetClassValue, );

    /**
     * Sets the enum value defined with the given name with the defined enum type. Implementation determines if values can be created (new) or only ever set based on a
     * previous definition (acting more like a class/object with defined properties)
     */
	UFUNCTION(BlueprintCallable, Category = "Rpai")
	virtual void SetEnum(FName ValueName, UEnum* Type, uint8 Value) PURE_VIRTUAL(URpaiState::SetEnum, );

    /**
     * Sets the float value defined with the given name. Implementation determines if values can be created (new) or only ever set based on a
     * previous definition (acting more like a class/object with defined properties)
     */
	UFUNCTION(BlueprintCallable, Category = "Rpai")
	virtual void SetFloat(FName ValueName, float Value) PURE_VIRTUAL(URpaiState::SetFloat, );

    /**
     * Sets the int value defined with the given name. Implementation determines if values can be created (new) or only ever set based on a
     * previous definition (acting more like a class/object with defined properties)
     */
	UFUNCTION(BlueprintCallable, Category = "Rpai")
	virtual void SetInt(FName ValueName, int32 Value) PURE_VIRTUAL(URpaiState::SetInt, );

    /**
     * Sets the name value defined with the given name. Implementation determines if values can be created (new) or only ever set based on a
     * previous definition (acting more like a class/object with defined properties)
     */
	UFUNCTION(BlueprintCallable, Category = "Rpai")
	virtual void SetNameValue(FName ValueName, FName Value) PURE_VIRTUAL(URpaiState::SetName, );

    /**
     * Sets the object value defined with the given name. Implementation determines if values can be created (new) or only ever set based on a
     * previous definition (acting more like a class/object with defined properties)
     */
	UFUNCTION(BlueprintCallable, Category = "Rpai")
	virtual void SetObject(FName ValueName, UObject* Value) PURE_VIRTUAL(URpaiState::SetObject, );

    /**
     * Sets the rotator value defined with the given name. Implementation determines if values can be created (new) or only ever set based on a
     * previous definition (acting more like a class/object with defined properties)
     */
	UFUNCTION(BlueprintCallable, Category = "Rpai")
	virtual void SetRotator(FName ValueName, FRotator Value) PURE_VIRTUAL(URpaiState::SetRotator, );

    /**
     * Sets the string value defined with the given name. Implementation determines if values can be created (new) or only ever set based on a
     * previous definition (acting more like a class/object with defined properties)
     */
	UFUNCTION(BlueprintCallable, Category = "Rpai")
	virtual void SetString(FName ValueName, FString Value) PURE_VIRTUAL(URpaiState::SetString, );

    /**
     * Sets the vector value defined with the given name. Implementation determines if values can be created (new) or only ever set based on a
     * previous definition (acting more like a class/object with defined properties)
     */
	UFUNCTION(BlueprintCallable, Category = "Rpai")
	virtual void SetVector(FName ValueName, FVector Value) PURE_VIRTUAL(URpaiState::SetVector, );

    /**
     * Gets the boolean value defined with the given name.
     *
     * @Param ValueName the key name of the value in the state to get
     * @Param OutValue reference to the value that will serve as storage for the copied value found in the state
     * @Returns true if the value was found and OutValue is valid for usage, false otherwise
     */
	UFUNCTION(BlueprintPure, Category = "Rpai")
	virtual bool GetBool(FName ValueName, bool& OutValue) const PURE_VIRTUAL(URpaiState::GetBool, return false;);

    /**
     * Gets the class value defined with the given name.
     *
     * @Param ValueName the key name of the value in the state to get
     * @Param OutValue reference to the value that will serve as storage for the copied value found in the state
     * @Returns true if the value was found and OutValue is valid for usage, false otherwise
     */
	UFUNCTION(BlueprintPure, Category = "Rpai")
	virtual bool GetClassValue(FName ValueName, UClass*& OutValue) const PURE_VIRTUAL(URpaiState::GetClassValue, return false;);

    /**
     * Gets the enum value defined with the given name and type
     *
     * @Param ValueName the key name of the value in the state to get
     * @Param OutValue reference to the value that will serve as storage for the copied value found in the state
     * @Returns true if the value was found and OutValue is valid for usage, false otherwise
     */
	UFUNCTION(BlueprintPure, Category = "Rpai")
	virtual bool GetEnum(FName ValueName, UEnum* Type, uint8& OutValue) const PURE_VIRTUAL(URpaiState::GetEnum, return false;);

    /**
     * Gets the float value defined with the given name.
     *
     * @Param ValueName the key name of the value in the state to get
     * @Param OutValue reference to the value that will serve as storage for the copied value found in the state
     * @Returns true if the value was found and OutValue is valid for usage, false otherwise
     */
	UFUNCTION(BlueprintPure, Category = "Rpai")
	virtual bool GetFloat(FName ValueName, float& OutValue) const PURE_VIRTUAL(URpaiState::GetFloat, return false;);

    /**
     * Gets the int value defined with the given name.
     *
     * @Param ValueName the key name of the value in the state to get
     * @Param OutValue reference to the value that will serve as storage for the copied value found in the state
     * @Returns true if the value was found and OutValue is valid for usage, false otherwise
     */
	UFUNCTION(BlueprintPure, Category = "Rpai")
	virtual bool GetInt(FName ValueName, int32& OutValue) const PURE_VIRTUAL(URpaiState::GetInt, return false;);

    /**
     * Gets the name value defined with the given name.
     *
     * @Param ValueName the key name of the value in the state to get
     * @Param OutValue reference to the value that will serve as storage for the copied value found in the state
     * @Returns true if the value was found and OutValue is valid for usage, false otherwise
     */
	UFUNCTION(BlueprintPure, Category = "Rpai")
	virtual bool GetNameValue(FName ValueName, FName& OutValue) const PURE_VIRTUAL(URpaiState::GetNameValue, return false;);

    /**
     * Gets the object value defined with the given name.
     *
     * @Param ValueName the key name of the value in the state to get
     * @Param OutValue reference to the value that will serve as storage for the copied value found in the state
     * @Returns true if the value was found and OutValue is valid for usage, false otherwise
     */
	UFUNCTION(BlueprintPure, Category = "Rpai")
	virtual bool GetObject(FName ValueName, UObject*& OutValue) const PURE_VIRTUAL(URpaiState::GetObject, return false;);

    /**
     * Gets the rotator value defined with the given name.
     *
     * @Param ValueName the key name of the value in the state to get
     * @Param OutValue reference to the value that will serve as storage for the copied value found in the state
     * @Returns true if the value was found and OutValue is valid for usage, false otherwise
     */
	UFUNCTION(BlueprintPure, Category = "Rpai")
	virtual bool GetRotator(FName ValueName, FRotator& OutValue) const PURE_VIRTUAL(URpaiState::GetRotator, return false;);

    /**
     * Gets the string value defined with the given name.
     *
     * @Param ValueName the key name of the value in the state to get
     * @Param OutValue reference to the value that will serve as storage for the copied value found in the state
     * @Returns true if the value was found and OutValue is valid for usage, false otherwise
     */
	UFUNCTION(BlueprintPure, Category = "Rpai")
	virtual bool GetString(FName ValueName, FString& OutValue) const PURE_VIRTUAL(URpaiState::GetString, return false;);

    /**
     * Gets the vector value defined with the given name.
     *
     * @Param ValueName the key name of the value in the state to get
     * @Param OutValue reference to the value that will serve as storage for the copied value found in the state
     * @Returns true if the value was found and OutValue is valid for usage, false otherwise
     */
	UFUNCTION(BlueprintPure, Category = "Rpai")
	virtual bool GetVector(FName ValueName, FVector& OutValue) const PURE_VIRTUAL(URpaiState::GetVector, return false;);

    /**
    * Check to verify the state has a boolean with the given name
    *
    * @Param ValueName the key name of the value in the state to verify
    * @Returns true if the value was with the given name of this type was found
    */
	UFUNCTION(BlueprintPure, Category = "Rpai")
	virtual bool HasBool(FName ValueName) const PURE_VIRTUAL(URpaiState::HasBool, return false;);

    /**
    * Check to verify the state has a class with the given name
    *
    * @Param ValueName the key name of the value in the state to verify
    * @Returns true if the value was with the given name of this type was found
    */
	UFUNCTION(BlueprintPure, Category = "Rpai")
	virtual bool HasClass(FName ValueName) const PURE_VIRTUAL(URpaiState::HasClass, return false;);

    /**
    * Check to verify the state has a enum with the given name
    *
    * @Param ValueName the key name of the value in the state to verify
    * @Returns true if the value was with the given name of this type was found
    */
	UFUNCTION(BlueprintPure, Category = "Rpai")
	virtual bool HasEnum(FName ValueName) const PURE_VIRTUAL(URpaiState::HasEnum, return false;);

    /**
    * Check to verify the state has a float with the given name
    *
    * @Param ValueName the key name of the value in the state to verify
    * @Returns true if the value was with the given name of this type was found
    */
	UFUNCTION(BlueprintPure, Category = "Rpai")
	virtual bool HasFloat(FName ValueName) const PURE_VIRTUAL(URpaiState::HasFloat, return false;);

    /**
    * Check to verify the state has a integer with the given name
    *
    * @Param ValueName the key name of the value in the state to verify
    * @Returns true if the value was with the given name of this type was found
    */
	UFUNCTION(BlueprintPure, Category = "Rpai")
	virtual bool HasInt(FName ValueName) const PURE_VIRTUAL(URpaiState::HasInt, return false;);

    /**
    * Check to verify the state has a name with the given name
    *
    * @Param ValueName the key name of the value in the state to verify
    * @Returns true if the value was with the given name of this type was found
    */
	UFUNCTION(BlueprintPure, Category = "Rpai")
	virtual bool HasName(FName ValueName) const PURE_VIRTUAL(URpaiState::HasName, return false;);

    /**
    * Check to verify the state has a object with the given name
    *
    * @Param ValueName the key name of the value in the state to verify
    * @Returns true if the value was with the given name of this type was found
    */
	UFUNCTION(BlueprintPure, Category = "Rpai")
	virtual bool HasObject(FName ValueName) const PURE_VIRTUAL(URpaiState::HasObject, return false;);

    /**
    * Check to verify the state has a rotator with the given name
    *
    * @Param ValueName the key name of the value in the state to verify
    * @Returns true if the value was with the given name of this type was found
    */
	UFUNCTION(BlueprintPure, Category = "Rpai")
	virtual bool HasRotator(FName ValueName) const PURE_VIRTUAL(URpaiState::HasRotator, return false;);

    /**
    * Check to verify the state has a string with the given name
    *
    * @Param ValueName the key name of the value in the state to verify
    * @Returns true if the value was with the given name of this type was found
    */
	UFUNCTION(BlueprintPure, Category = "Rpai")
	virtual bool HasString(FName ValueName) const PURE_VIRTUAL(URpaiState::HasString, return false;);

    /**
    * Check to verify the state has a vector with the given name
    *
    * @Param ValueName the key name of the value in the state to verify
    * @Returns true if the value was with the given name of this type was found
    */
	UFUNCTION(BlueprintPure, Category = "Rpai")
	virtual bool HasVector(FName ValueName) const PURE_VIRTUAL(URpaiState::HasVector, return false;);

    /**
    * Check to verify the state has a any type of value with the given name
    *
    * @Param ValueName the key name of the value in the state to verify
    * @Returns true if the value was with the given name of this type was found
    */
	UFUNCTION(BlueprintPure, Category = "Rpai")
	virtual bool HasValueWithName(FName ValueName) const PURE_VIRTUAL(URpaiState::HasValueWithName, return false;);

    /**
    * Check to verify the state has a boolean with the given name
    *
    * @Param ValueName the key name of the value in the state to verify
    * @Returns true if the value was with the given name of this type was found
    */
	UFUNCTION(BlueprintPure, Category = "Rpai")
	virtual EStatePropertyType GetTypeFromName(FName ValueName) const PURE_VIRTUAL(URpaiState::GetTypeFromName, return EStatePropertyType::Invalid;);

    /**
    * Check to verify the state has a boolean with the given name
    *
    * @Param ValueName the key name of the value in the state to verify
    * @Returns true if the value was with the given name of this type was found
    */
	UFUNCTION(BlueprintPure, Category = "Rpai")
	bool HasReferencedState(const FStateKeyValueReference & StatePropertyReference) const;

    /**
    * Takes all state properties needed for planning and prediction and copies them to the given state.
    *
    * @Param OtherState the state to copy values to
    */
	UFUNCTION(BlueprintCallable, Category = "Rpai")
	virtual void CopyStateForPredictionTo(URpaiState* OtherState) const PURE_VIRTUAL(URpaiState::CopyStateForPreditionTo,);

	/**
	* Query if the given resource is locked by any object
	*/
	UFUNCTION(BlueprintPure, Category = "ResonablePlanningAI")
	virtual bool IsResourceLocked(FName ExactName) const PURE_VIRTUAL(URpaiState::IsResourceLocked, return false;);

	/**
	* Query if the given resource is not locked by any object
	*/
	UFUNCTION(BlueprintPure, Category = "ResonablePlanningAI")
	virtual bool IsResourceFree(FName ExactName) const PURE_VIRTUAL(URpaiState::IsResourceFree, return false;);

	/**
	* Query if the given path prefix of a collection of resources are locked
	*/
	UFUNCTION(BlueprintCallable, Category = "Rpai")
	virtual bool DoesHeirarchyHaveAnyLocks(FName PathPrefix) const PURE_VIRTUAL(URpaiState::DoesHeirarchyHaveAnyLocks, return false;);

	/**
	* Locks the resource to the given LockedBy as the original Locker. Returns
	* true if the resource is successfully locked with given valid params. Will
	* Lazily create a resource if it does not exist.
	*/
	UFUNCTION(BlueprintCallable, Category = "ResonablePlanningAI")
	virtual bool LockResource(FName ExactName, UObject* OptionalLockingObject = nullptr) PURE_VIRTUAL(URpaiState::LockResource, return false;);

	/**
	* Unlocks the resource if the give LockedBy is the original Locker. Returns
	* true if the resource is successfully unlocked with given valid params.
	*/
	UFUNCTION(BlueprintCallable, Category = "ResonablePlanningAI")
	virtual bool UnlockResource(FName ExactName, UObject* OptionalLockingObject = nullptr) PURE_VIRTUAL(URpaiState::UnlockResource, return false;);

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
