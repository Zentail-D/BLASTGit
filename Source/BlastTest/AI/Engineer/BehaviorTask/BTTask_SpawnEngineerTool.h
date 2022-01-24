// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyControllerParent.h"
#include "EngineerEnemy.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_SpawnEngineerTool.generated.h"


/**
 * 
 */
UCLASS()
class  UBTTask_SpawnEngineerTool : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	/**
	* Set to notify on tick so that TickTask is called every frame. And sets the Node Name
	*/
	UBTTask_SpawnEngineerTool(FObjectInitializer const& object_initializer);
	/**
	* Empty the location we are going to so that we can create new locations. Set the timer for Spawning in the Tools
	* @param OwnerComp Get Reference to Current Enemy and Controller
	* @param NodeMemory Needed to override the Function
	*/
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	/**
	* When the Timer runs out it will call the spawn wave function
	* @param OwnerComp Get Reference to Current Enemy and Controller
	* @param NodeMemory Needed to override the Function
	* @param DeltaSeconds The amount of time that has passed since the last call of this function
	*/
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	

private:
	/*Blackboard key FiringCooldown set for starting the process of spawning in tools*/
	UPROPERTY(EditAnywhere, Category = Blackboard, meta = (AllowPrivateAccess = "true"))
	struct FBlackboardKeySelector FiringCooldown;

	/*Blackboard key SpawningTimer the timer after the process is started to spawn in a actual tool*/
	UPROPERTY(EditAnywhere, Category = Blackboard, meta = (AllowPrivateAccess = "true"))
	struct FBlackboardKeySelector SpawningTimer;

	/**
	 * How far from the current location the random location will be found
	 */
	UPROPERTY(EditAnywhere, Category = Blackboard, meta = (AllowPrivateAccess = "true"))
	float OffSetFromPathingToSpawn;

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
	 * Spawn in a Random Tool at a location created in the function. Tools are determined by what is attached to the engineer tools array
	 */
	void SpawnRandomTool() const;

	/**
	* @return Returns how far from the End Destination Drone is
	*/
	float DistanceToFinal() const;

};
