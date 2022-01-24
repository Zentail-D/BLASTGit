// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <stdbool.h>

#include "CoreMinimal.h"
#include "EnemyControllerParent.h"
#include "EngineerEnemy.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_SpawnDrone.generated.h"

/**
 * 
 */
UCLASS()
class  UBTTask_SpawnDrone : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	/**
	* Set to notify on tick so that TickTask is called every frame. And sets the Node Name
	*/
	UBTTask_SpawnDrone(FObjectInitializer const& object_initializer);
	/**
	* Sets the spawn time for the drone. The Spawning timer is used. It initalizes it in the blackboard. Instant Spawn will override this value and make the drone spawn in instantly
	* @param OwnerComp Get Reference to Current Enemy and Controller
	* @param NodeMemory Needed to override the Function
	*/
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	/**
	* Spawns in a certain number of drone based off of Number of Drone to Spawn
	* @param OwnerComp Get Reference to Current Enemy and Controller
	* @param NodeMemory Needed to override the Function
	* @param DeltaSeconds The amount of time that has passed since the last call of this function
	*/
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	

private:
	/**Blackboard Key used to set the spawning timer for the Drones*/
	UPROPERTY(EditAnywhere, Category = Blackboard, meta = (AllowPrivateAccess = "true"))
	struct FBlackboardKeySelector SpawningTimer;

	/**Blackboard Key used to set the Can see target*/
	UPROPERTY(EditAnywhere, Category = Blackboard, meta = (AllowPrivateAccess = "true"))
	struct FBlackboardKeySelector CanSeeTarget;
	
	/**
	* The Object type we want to check in our box trace normally will be set to Dynamic and Static
	*/
	UPROPERTY(EditAnywhere, Category = Blackboard, meta = (AllowPrivateAccess = "true"))
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

	/**
	* The Filter to be checking when doing the box trace normally set to Actor
	*/
	UPROPERTY(EditAnywhere, Category = Blackboard, meta = (AllowPrivateAccess = "true"))
	UClass* ActorFilterClass;

	/**
	 * Boolean that will make the drone instantly spawn. false for need timer and true for instant
	 */
	UPROPERTY(EditAnywhere, Category = Blackboard, meta = (AllowPrivateAccess = "true"))
	bool bInstantSpawn=false;

	/**
	 * How far from the engineer the drone will spawn
	 */
	UPROPERTY(EditAnywhere, Category = Blackboard, meta = (AllowPrivateAccess = "true"))
	float RandomDistanceFromEngineer= 200;

	/**
	 * Number of drone to spawn in a wave
	 */
	UPROPERTY(EditAnywhere, Category = Blackboard, meta = (AllowPrivateAccess = "true"))
	float NumberOfDroneToSpawn= 1;

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
	 *Helper function for RandomLocationAroundEngineer
	 *@param DesiredLocation the location to do the collision check
	 * @return Return true if there was a collision and false if there wasnt
	 */
	bool CheckCollision(FVector DesiredLocation) const;

	/**
	 * Helper function for SpawnDrones
	 * @return Return the location to spawn the drone at
	 */
	FVector RandomLocationAroundEngineer() const;

	/**
	 * Spawns in a specified number of drones
	 * @param NumOfDrones The number of drone to be spawned in
	 */
	void SpawnDrones(int NumOfDrones = 1) const;
};
