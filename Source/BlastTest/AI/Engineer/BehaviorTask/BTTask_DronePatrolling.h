// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyControllerParent.h"
#include "EngineerDrones.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_DronePatrolling.generated.h"

/**
 * 
 */
UCLASS()
class  UBTTask_DronePatrolling : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:

	/**
	* Set to notify on tick so that TickTask is called every frame. And sets the Node Name
	*/
	UBTTask_DronePatrolling(FObjectInitializer const& object_initializer);
	/**
	*Always in progress
	* @param OwnerComp Get Reference to Current Enemy and Controller
	* @param NodeMemory Needed to override the Function
	*/
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	/**
	* Patrols to a point and then returns and then runs the task over and over again. Checks to see if it can reach the locaiton it wants to go to
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
	/**
	* This is the Distance from the end point when we enter the node used in calculating the force vector;
	*/
	float InitialFinalDistance;
	
	/**
	* @return Returns how far from the End Destination Drone is
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
	 * Helper function to rotate it causes lag so it is commeted out
	 */
	void RotateToFaceMovingDirection() const;
};
