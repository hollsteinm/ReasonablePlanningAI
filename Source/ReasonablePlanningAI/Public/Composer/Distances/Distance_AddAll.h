// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/ReasonablePlanningDistance.h"
#include "Distance_AddAll.generated.h"

/**
 * All sub distances are returned as a summation. Due to bad design, LHS for this instance is ignored.
 */
UCLASS()
class REASONABLEPLANNINGAI_API UDistance_AddAll : public UReasonablePlanningDistance
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "RPAI", Instanced)
	TArray<UReasonablePlanningDistance*> SubDistances;

	virtual float ReceiveCalculateDistance_Implementation(const UReasonablePlanningState* GivenState) const override;

public:
	FORCEINLINE void SetSubDistances(TArray<UReasonablePlanningDistance*>&& NewSetSubDistances) { SubDistances = NewSetSubDistances; }
};
