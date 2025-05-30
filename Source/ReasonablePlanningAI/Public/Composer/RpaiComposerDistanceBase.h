// Radaway Software LLC. 2025. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RpaiComposerDistanceBase.generated.h"

class URpaiState;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced)
class REASONABLEPLANNINGAI_API URpaiComposerDistanceBase : public UObject
{
	GENERATED_BODY()

public:
	/**
	* Public interface to determine distance between two compared values in a give ReasonablePlanningState
	*/
	UFUNCTION(BlueprintPure, Category = "Rpai")
		float CalculateDistance(const URpaiState* GivenState) const;

protected:
	/**
	* Override to implement CalculateDistance
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Rpai")
		float ReceiveCalculateDistance(const URpaiState* GivenState) const;
	virtual float ReceiveCalculateDistance_Implementation(const URpaiState* GivenState) const;
	
};
