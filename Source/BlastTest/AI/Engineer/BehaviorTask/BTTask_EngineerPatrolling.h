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
	/**Blackboard key to set the character index to rotate towards*/
	UPROPERTY(EditAnywhere, Category = Blackboard, meta = (AllowPrivateAccess = "true"))
	struct FBlackboardKeySelector NotMovingTimer;

	
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

	/**
	* @return Returns how far from the End Destination Engineer is
	*/
	float DistanceToFinal() const;
};
