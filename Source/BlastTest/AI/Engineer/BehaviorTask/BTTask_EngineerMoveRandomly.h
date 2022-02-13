// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyControllerParent.h"
#include "EngineerEnemy.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_EngineerMoveRandomly.generated.h"

/**
 * 
 */
UCLASS()
class  UBTTask_EngineerMoveRandomly : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:

	/**
	* Set to notify on tick so that TickTask is called every frame. And sets the Node Name
	*/
	UBTTask_EngineerMoveRandomly(FObjectInitializer const& object_initializer);
	/**
	* Empties the Destination to allow movement
	* @param OwnerComp Get Reference to Current Enemy and Controller
	* @param NodeMemory Needed to override the Function
	*/
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	/**
	* Moves to a random location over and over again only gets out when it leaves the sight range of the player
	* @param OwnerComp Get Reference to Current Enemy and Controller
	* @param NodeMemory Needed to override the Function
	* @param DeltaSeconds The amount of time that has passed since the last call of this function
	*/
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	

private:
	/**
	 * The distance for the AI to travel when finding location
	 */
	UPROPERTY(EditAnywhere, Category = Blackboard, meta = (AllowPrivateAccess = "true"))
	float DistanceOfEachPathing =300;

	/**
	* Saved off reference to the Engineer to ensure that we dont have to do multiple casts
	*@warning You have to check if the pointer is null
	*/
	AEngineerEnemy* Engineer;
	/**
	* Saved off reference to the Controller to ensure that we dont have to do multiple casts
	*@warning You have to check if the pointer is null
	*/
	AEnemyControllerParent* EnemyController;

	/**
	* Helper function for  Find Location Near Player
	* @return return the player index for the player we want to find a location near
	*/
	int FindClosestPlayer() const;

	/**
	 * Move to location near player using the internal engineer variable to know what distance to keep from the player
	 * @returns true if successful and false if it was unsuccessful
	 */
	bool MoveToLocationNearPlayer() const;

};
