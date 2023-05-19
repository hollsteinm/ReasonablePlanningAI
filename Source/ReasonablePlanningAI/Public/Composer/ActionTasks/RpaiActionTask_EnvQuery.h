// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/RpaiComposerActionTask.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "BrainComponent.h"
#include "RpaiActionTask_EnvQuery.generated.h"

USTRUCT(BlueprintType)
struct REASONABLEPLANNINGAI_API FActionTaskEnvQueryMemory
{
	GENERATED_BODY()

	FActionTaskEnvQueryMemory();

	int32 RequestId;
};

/**
 * Run an environment query during runtime to make you AI a bit more dynamic. This does throw off your hueristcs a bit, so the plan may not make sense
 * if you run this often.
 */
UCLASS()
class REASONABLEPLANNINGAI_API URpaiActionTask_EnvQuery : public URpaiComposerActionTask
{
	GENERATED_BODY()

public:
	URpaiActionTask_EnvQuery();

	UPROPERTY(Category=Rpai, EditAnywhere)
	TObjectPtr<UEnvQuery> QueryTemplate;

	UPROPERTY(Category = Rpai, EditAnywhere)
	TArray<FAIDynamicParam> QueryConfig;

	UPROPERTY(Category = Rpai, EditAnywhere)
	TEnumAsByte<EEnvQueryRunMode::Type> RunMode;

private:
	/* Rare managed runtime state property. Used to maintain references to these handles less they go out of scope. */
	TArray<FAIMessageObserverHandle> ObserverCache;

	void OnQueryFinished(TSharedPtr<FEnvQueryResult> Result, AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory);
	void OnAIMessage(UBrainComponent* BrainComp, const FAIMessage& Message, AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory);

protected:
	virtual void ReceiveStartActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr) override;
	virtual void ReceiveCancelActionTask_Implementation(AAIController* ActionInstigator, URpaiState* CurrentState, FRpaiMemoryStruct ActionMemory, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr, bool bCancelShouldExitPlan = true) override;
};
