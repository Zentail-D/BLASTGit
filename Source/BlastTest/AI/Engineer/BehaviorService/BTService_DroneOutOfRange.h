// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyControllerParent.h"
#include "EngineerDrones.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_DroneOutOfRange.generated.h"

/**
 * 
 */
UCLASS()
class  UBTService_DroneOutOfRange : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	/**
	 * Sets the Service to notify on become relavent and ensure that the name of the service is set
	 */
	UBTService_DroneOutOfRange();
	/**
	 * Runs the like execute task in a BTTask.
	 * This service will check to see if a drone is out of range and set the InRange to false.
	 * This Service will not change in range to true it has to be set when it finds the location it wants to go to
	 * @param OwnerComp Gives reference to the current Player and Controller
	 * @param NodeMemory Parameter need for override
	 */
	void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
protected:
	/**
	* This will be what a get service description will return in c++ code
	*/
	FString GetStaticServiceDescription() const override;

private:
	/**
	* The In Range Blackboard Key that inform the drone if it can continue shooting at the player
	*/
	UPROPERTY(EditAnywhere, Category = Blackboard, meta = (AllowPrivateAccess = "true"))
	struct FBlackboardKeySelector InRange;

	/**
	* Saved off reference to the Controller to ensure that we dont have to do multiple casts
	*@warning You have to check if the pointer is null
	*/
	AEnemyControllerParent* EnemyController;
	/**
	* Saved off reference to the Drone to ensure that we dont have to do multiple casts
	*@warning You have to check if the pointer is null
	*/
	AEngineerDrones* EngineerDrone;

	/**
	 *Helper function to find if the Drone is  in range of the Enemy
	* @return Return True if the player is in range and false if it is out of range
	* @warning This will do GEngine Prints if there are not any players set
	*/
	bool FindPlayerInRange();
};
