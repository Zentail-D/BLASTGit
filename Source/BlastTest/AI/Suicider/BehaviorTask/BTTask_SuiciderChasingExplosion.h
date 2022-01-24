// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyControllerParent.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Suicider/SuiciderEnemy.h"
#include "BTTask_SuiciderChasingExplosion.generated.h"

/**
 * 
 */
UCLASS()
class  UBTTask_SuiciderChasingExplosion : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	
public:
	/**
	* Set to notify on tick so that TickTask is called every frame. And sets the Node Name
	*/
	UBTTask_SuiciderChasingExplosion(FObjectInitializer const& object_initializer);
	/**
	* Get the timer for the Explosion and starts the suicider moving towards the player
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
	 * Boolean that starts the Suicider Charge up attack
	 */
	bool bStartChargeUpAttack = false;

	/**
	 * If the Suicider Reaches this time it will explode no matter what
	 * The Point of No Return for the Suicider
	 */
	UPROPERTY(EditAnywhere, Category = Blackboard, meta = (AllowPrivateAccess = "true"))
	float InstantDeathTime = 0.2;
	
	/**Blackboard key to set the character index to rotate towards*/
	UPROPERTY(EditAnywhere, Category = Blackboard, meta = (AllowPrivateAccess = "true"))
	struct FBlackboardKeySelector AttackingCooldown;

	/**Blackboard key to set the character index to rotate towards*/
	UPROPERTY(EditAnywhere, Category = Blackboard, meta = (AllowPrivateAccess = "true"))
	struct FBlackboardKeySelector TargetCharacterIndex;
	
	/**
	* Saved off reference to the Suicider to ensure that we dont have to do multiple casts
	*@warning You have to check if the pointer is null
	*/
	ASuiciderEnemy* SuiciderEnemy;
	/**
	* Saved off reference to the Controller to ensure that we dont have to do multiple casts
	*@warning You have to check if the pointer is null
	*/
	AEnemyControllerParent* EnemyController;

	/**
	* @return Returns how far from the End Destination Engineer is
	*/
	float DistanceToFinal() const;

	/**
	 * Call this when the Suicider Deals damage that will be used for implementing the different Colors of suicider
	 */
	void SuiciderAttackingBehavior() const;

	/**
	 * Check Which Player is Closer
	 * @return Return a index to the player that is closer
	 */
	int GetClosestPlayer() const;

	/**
	 * Helper Function For Suicider Attacking Behavior
	 * Deals damage to any player in range of the suicider
	 */
	void SuiciderExplosion() const;
};
