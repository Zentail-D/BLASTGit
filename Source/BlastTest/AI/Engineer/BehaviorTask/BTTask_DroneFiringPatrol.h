// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyControllerParent.h"
#include "EngineerDrones.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_DroneFiringPatrol.generated.h"

/**
 * 
 */
UCLASS()
class  UBTTask_DroneFiringPatrol : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	/**
	 * Set to notify on tick so that TickTask is called every frame. And sets the Node Name
	 */
	UBTTask_DroneFiringPatrol(FObjectInitializer const& object_initializer);
	/**
	 * Get the Firing Time from the Drone and sets it to the Blackboard
	 * @param OwnerComp Get Reference to Current Enemy and Controller
	 * @param NodeMemory Needed to override the Function
	 */
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	/**
	* Fires bullets from the Drone and Moves around in its claimed space
	* @param OwnerComp Get Reference to Current Enemy and Controller
	* @param NodeMemory Needed to override the Function
	* @param DeltaSeconds The amount of time that has passed since the last call of this function
	*/
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;


private:
	/**
	* Saved off reference to the Drone to ensure that we dont have to do multiple casts
	*@warning You have to check if the pointer is null
	*/
	AEngineerDrones* EngineerDrone;

	/**
	* Saved off reference to the Controller to ensure that we dont have to do multiple casts
	*@warning You have to check if the pointer is null
	*/
	AEnemyControllerParent* EnemyController;
	
	/**Blackboard key to set the character to rotate towards*/
	UPROPERTY(EditAnywhere, Category = Blackboard, meta = (AllowPrivateAccess = "true"))
	struct FBlackboardKeySelector TargetCharacter;

	/**Blackboard key to set the character index to rotate towards*/
	UPROPERTY(EditAnywhere, Category = Blackboard, meta = (AllowPrivateAccess = "true"))
	struct FBlackboardKeySelector TargetCharacterIndex;

	/**Blackboard key to set the character index to rotate towards*/
	UPROPERTY(EditAnywhere, Category = Blackboard, meta = (AllowPrivateAccess = "true"))
	struct FBlackboardKeySelector FiringCooldown;

	/**Blackboard key to Center of the claimed space*/
	UPROPERTY(EditAnywhere, Category = Blackboard, meta = (AllowPrivateAccess = "true"))
	struct FBlackboardKeySelector DroneInRangeLocation;

	/**
	 * Finds a random location in the claimed space for the drone to move to
	 */
	void FindLocationNearBlackBoardKey() const;

	/**
	 * @return Give how far from the Destination the Drone is
	 */
	float DistanceToFinal() const;

	/**
	 * @return Gives the Direction in which the Force needs to be applied in
	 */
	FVector ForceVector() const;

	/**
	 * @return Gives true if we can add more force or false if we are already going to the fastest
	 */
	bool BelowMaxVelocity() const;

	/**
	 * This is the Distance from the end point when we enter the node used in calculating the force vector;
	 */
	float InitialFinalDistance;
};
