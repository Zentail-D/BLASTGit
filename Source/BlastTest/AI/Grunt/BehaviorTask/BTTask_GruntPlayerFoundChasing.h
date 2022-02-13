// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GruntAIEnemy.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_GruntPlayerFoundChasing.generated.h"


/**
 * 
 */
UCLASS()
class  UBTTask_GruntPlayerFoundChasing : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	// Constructor to set up the node name and anything else
	UBTTask_GruntPlayerFoundChasing(FObjectInitializer const& ObjectInitializer);
	
	/** main task function that gets ran from BT */
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	/** what will get called every run through of the behavior tree */
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	/** Int that represents the current index were at for the patrol path */
	UPROPERTY(EditAnywhere, Category = Blackboard, meta = (AllowPrivateAccess = "true"))
	struct FBlackboardKeySelector NeedNewPatrolPoint;
	
	/** FVector to the location wanted*/
	UPROPERTY(EditAnywhere, Category = Blackboard, meta = (AllowPrivateAccess = "true"))
	struct FBlackboardKeySelector CanSeePlayer;


private:
	/**
	* Check Which Player is Closer
	* @return Return a index to the player that is closer
	*/
	int GetClosestPlayer(AGruntAIEnemy* GruntAI) const;

	/**Grunt Move contained in a function that find the location near the player to how towards it and splits the move to in multple chucks so that the AI will progress over a path
	 * @returns false if it fails to create a location and true if it succeeds
	 */
	bool MoveToLocation(AGruntAIEnemy* GruntAI, class AEnemyControllerParent* EnemyController) const;

	/**
	 * Internal Function used to create the strafing path that the grunt uses
	 * @param GruntAI used to get a reference to the grunts stats so that we can set the strafing path and create the points leading up to the EndLocation
	 * @param EndLocation The location that the grunt is attempting to go towards
	 */
	static void CreatePath(AGruntAIEnemy* GruntAI, FVector EndLocation);
	
};


