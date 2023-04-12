// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RpaiStateBlackboardComponent.generated.h"

/*
* Actor component used for any AI that use Reasonable Planning AI in tandem with Behavior Trees. Needed to derive state obects
* from the Blackboard Component.
*/
UCLASS( ClassGroup=(Rpai), meta=(BlueprintSpawnableComponent) )
class REASONABLEPLANNINGAI_API URpaiStateBlackboardComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category = "Rpai")
	class URpaiState_BlackboardComponentAdapter* StateAdapter;

public:	
	// Sets default values for this component's properties
	URpaiStateBlackboardComponent();

	FORCEINLINE URpaiState_BlackboardComponentAdapter* GetStateAdapter() const { return StateAdapter; }
};
