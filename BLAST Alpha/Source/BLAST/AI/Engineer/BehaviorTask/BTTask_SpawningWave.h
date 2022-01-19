// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyControllerParent.h"
#include "EngineerSpawner.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_SpawningWave.generated.h"

/**
 * 
 */
UCLASS()
class BLAST_API UBTTask_SpawningWave : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	/**
	* Set to notify on tick so that TickTask is called every frame. And sets the Node Name
	*/
	UBTTask_SpawningWave(FObjectInitializer const& object_initializer);
	/**
	* Set the Blackboard Firing Cooldown to the Time till next spawn in the spawner
	* @param OwnerComp Get Reference to Current Enemy and Controller
	* @param NodeMemory Needed to override the Function
	*/
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	/**
	* Move around randomly and finish the task and Stop movement and empty destination when spawn in a tool.
	* @param OwnerComp Get Reference to Current Enemy and Controller
	* @param NodeMemory Needed to override the Function
	* @param DeltaSeconds The amount of time that has passed since the last call of this function
	*/
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;


private:
	/**
	* Saved off reference to the Spawner to ensure that we dont have to do multiple casts
	*@warning You have to check if the pointer is null
	*/
	AEngineerSpawner* EngineerSpawner;
	
	/**
	* Saved off reference to the Controller to ensure that we dont have to do multiple casts
	*@warning You have to check if the pointer is null
	*/
	AEnemyControllerParent* EnemyController;

	/*Blackboard key to Firing Cooldown to save off the timer*/
	UPROPERTY(EditAnywhere, Category = Blackboard, meta = (AllowPrivateAccess = "true"))
	struct FBlackboardKeySelector FiringCooldown;
};
