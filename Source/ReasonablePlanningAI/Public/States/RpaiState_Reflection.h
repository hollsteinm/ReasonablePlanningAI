// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/RpaiState.h"
#include "RpaiState_Reflection.generated.h"

class URpaiResourceCollection;

/**
 * A state that uses reflection as the implementation of the getter and setting and query methods
 * upon itself. Inherit from this class and add UPROPERTY fields to integrate. The UPROPERTY field MUST have metadata of "Rpai"
 */
UCLASS(Abstract)
class REASONABLEPLANNINGAI_API URpaiState_Reflection : public URpaiState
{
	GENERATED_BODY()
private:

	UPROPERTY(SaveGame)
	URpaiResourceCollection* Resources;

public:
	URpaiState_Reflection();
	
	virtual bool IsEqualTo(const URpaiState* OtherState) const override;
	virtual void SetBool(FName ValueName, bool Value) override;
	virtual void SetClassValue(FName ValueName, UClass* Value) override;
	virtual void SetEnum(FName ValueName, UEnum* Type, uint8 Value) override;
	virtual void SetFloat(FName ValueName, float Value) override;
	virtual void SetInt(FName ValueName, int32 Value) override;
	virtual void SetNameValue(FName ValueName, FName Value) override;
	virtual void SetObject(FName ValueName, UObject* Value) override;
	virtual void SetRotator(FName ValueName, FRotator Value) override;
	virtual void SetString(FName ValueName, FString Value) override;
	virtual void SetVector(FName ValueName, FVector Value) override;
	virtual bool GetBool(FName ValueName, bool& OutValue) const override;
	virtual bool GetClassValue(FName ValueName, UClass*& OutValue) const override;
	virtual bool GetEnum(FName ValueName, UEnum* Type, uint8& OutValue) const override;
	virtual bool GetFloat(FName ValueName, float& OutValue) const override;
	virtual bool GetInt(FName ValueName, int32& OutValue) const override;
	virtual bool GetNameValue(FName ValueName, FName& OutValue) const override;
	virtual bool GetObject(FName ValueName, UObject*& OutValue) const override;
	virtual bool GetRotator(FName ValueName, FRotator& OutValue) const override;
	virtual bool GetString(FName ValueName, FString& OutValue) const override;
	virtual bool GetVector(FName ValueName, FVector& OutValue) const override;
	virtual bool HasBool(FName ValueName) const override;
	virtual bool HasClass(FName ValueName) const override;
	virtual bool HasEnum(FName ValueName) const override;
	virtual bool HasFloat(FName ValueName) const override;
	virtual bool HasInt(FName ValueName) const override;
	virtual bool HasName(FName ValueName) const override;
	virtual bool HasObject(FName ValueName) const override;
	virtual bool HasRotator(FName ValueName) const override;
	virtual bool HasString(FName ValueName) const override;
	virtual bool HasVector(FName ValueName) const override;
	virtual bool HasValueWithName(FName ValueName) const override;
	virtual EStatePropertyType GetTypeFromName(FName ValueName) const override;
	virtual void CopyStateForPredictionTo(URpaiState* OtherState) const override;
	virtual bool IsResourceLocked(FName ExactName) const override;
	virtual bool IsResourceFree(FName ExactName) const override;
	virtual bool DoesHeirarchyHaveAnyLocks(FName PathPrefix) const override;
	virtual bool LockResource(FName ExactName, UObject* LockingObject = nullptr) override;
	virtual bool UnlockResource(FName ExactName, UObject* LockingObject = nullptr) override;
};
