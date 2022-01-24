// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FlyingEnemyAI.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "FlyingEnemyAIController.h"
#include "FindPathToAdvanceTowardsPlayer.generated.h"

/**
 * 
 */
UCLASS()
class  UFindPathToAdvanceTowardsPlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	/** Constructor that mostly just sets up the node name */
	UFindPathToAdvanceTowardsPlayer();
	
	/** main task function that gets ran from BT */
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory) override;
	
	/** FVector to the location wanted*/
	UPROPERTY(EditAnywhere, Category = Blackboard, meta = (AllowPrivateAccess = "true"))
	struct FBlackboardKeySelector NeedNewAttackPath;
	
	/** FVector to the location wanted*/
	UPROPERTY(EditAnywhere, Category = Blackboard, meta = (AllowPrivateAccess = "true"))
	struct FBlackboardKeySelector NeedNewPatrolPath;

	/**Object types that need to be checked for collision*/
	UPROPERTY(EditAnywhere, Category = Blackboard, meta = (AllowPrivateAccess = "true"))
	TArray<TEnumAsByte<EObjectTypeQuery>> object_types;

	/**Class of actors to filer out in the path finding*/
	UPROPERTY(EditAnywhere, Category = Blackboard, meta = (AllowPrivateAccess = "true"))
	UClass* actorclassfilter;

private:
	/** Gets a random point perpendicular to the direction vector from player to enemy
	 * @param MyEnemy - Reference to the enemy character
	 * @return The point in space chosen
	 */
	FVector GetRandomPointAroundPlayer(AFlyingEnemyAI* MyEnemy);
	
};
