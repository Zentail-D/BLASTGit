// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyControllerParent.h"
#include "EngineerEnemy.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_EngineerPatrolling.generated.h"

/**
 * 
 */
UCLASS()
class  UBTTask_EngineerPatrolling : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
    
public:
	/**
	* Set to notify on tick so that TickTask is called every frame. And sets the Node Name
	*/
	UBTTask_EngineerPatrolling(FObjectInitializer const& object_initializer);
	/**
	* Always in progress
	* @param OwnerComp Get Reference to Current Enemy and Controller
	* @param NodeMemory Needed to override the Function
	*/
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	/**
	* Finds random location around the patrol point to move to repeat
	* @param OwnerComp Get Reference to Current Enemy and Controller
	* @param NodeMemory Needed to override the Function
	* @param DeltaSeconds The amount of time that has passed since the last call of this function
	*/
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
    
private:

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
	 * Distance that the random location and size of the vector
	 */
	UPROPERTY(EditAnywhere, Category = Blackboard, meta = (AllowPrivateAccess = "true"))
	int OffsetFromPatrolPoint = 100;

	/**Internal Function to get the Engineer to move to a patrolling point with a offset so that they are not heading to the exact same location
	* @return False if we failed at finding the point true if we succeed
	*/
	bool MoveToPatrollingPoint() const;

};
