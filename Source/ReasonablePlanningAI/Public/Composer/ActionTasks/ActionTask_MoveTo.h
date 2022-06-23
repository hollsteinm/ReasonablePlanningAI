// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Composer/ReasonablePlanningActionTask.h"
#include "NavFilters/NavigationQueryFilter.h"
#include "BrainComponent.h"
#include "GameplayTask.h"
#include "ActionTask_MoveTo.generated.h"

/**
 * Will attempt a move to a defined location. Can use either the navigation system or
 * the AITask_MoveTo Gameplay Task.
 */
UCLASS(config = Game)
class REASONABLEPLANNINGAI_API UActionTask_MoveTo : public UReasonablePlanningActionTask
{
	GENERATED_BODY()

public:
	UActionTask_MoveTo();

protected:
	virtual void ReceiveStartActionTask_Implementation(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr) override;
	virtual void ReceiveUpdateActionTask_Implementation(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, float DeltaSeconds, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr) override;
	virtual void ReceiveCancelActionTask_Implementation(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState, AActor* ActionTargetActor = nullptr, UWorld* ActionWorld = nullptr) override;

	/** Fixed distance to determine if destination is reached */
	UPROPERTY(config, Category = "RPAI", EditAnywhere, meta = (ClampMin = "0.0", UIMin = "0.0"))
	float AcceptableRadius;

	/** "None" will result in default filter being used */
	UPROPERTY(Category = "RPAI", EditAnywhere)
	TSubclassOf<UNavigationQueryFilter> FilterClass;

	/** Allow left-right movement with different facing */
	UPROPERTY(Category = "RPAI", EditAnywhere)
	bool bAllowStrafe;

	/** Move somewhere close enough if the destination is unreachable */
	UPROPERTY(Category = "RPAI", EditAnywhere, AdvancedDisplay)
	bool bAllowPartialPath;

	/** Will update pathing with moving target */
	UPROPERTY(Category = "RPAI", EditAnywhere, AdvancedDisplay)
	bool bTrackMovingGoal;

	/** Location will be projected to navmesh */
	UPROPERTY(Category = "RPAI", EditAnywhere, AdvancedDisplay)
	bool bProjectGoalLocation;

	/** AI bounds will be included to detect reaching of destination */
	UPROPERTY(Category = "RPAI", EditAnywhere)
	bool bReachTestIncludesAgentRadius;

	/** Goal location capsule will be used to deteact reaching of destination */
	UPROPERTY(Category = "RPAI", EditAnywhere)
	bool bReachTestIncludesGoalRadius;

	/** Use more direct movement to reach destination */
	UPROPERTY(Category = "RPAI", EditAnywhere)
	bool bUsePathfinding;

	UPROPERTY()
	bool bStopOnOverlapNeedsUpdate;

private:
	void StartMoveActionTask(AAIController* ActionInstigator, UReasonablePlanningState* CurrentState);
	void OnAIMessage(UBrainComponent* BrainComp, const FAIMessage& Message, AAIController* ActionInstigator, UReasonablePlanningState* CurrentState);
};
