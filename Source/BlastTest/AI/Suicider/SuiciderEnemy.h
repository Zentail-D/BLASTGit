// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIEnemyParent.h"
#include "SuiciderAttackEnums.h"
#include "SuiciderEnemy.generated.h"

/**
 * 
 */
UCLASS()
class  ASuiciderEnemy : public AAIEnemyParent
{
	GENERATED_BODY()

public:
	/**
	* Enable the tick for the drone swarm
	* Initialize the 
	*/
	ASuiciderEnemy();


	/**
	* Boolean on whether or not the suicider is moving
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Suicider", meta = (AllowPrivateAccess = "true"))
	bool bSuiciderMoving = true;
	
	/**
	*Called every frame
	* Checks to see if it needs to be destroy and will Destroy it unless it bNeverDestroy is true
	*/
	virtual void Tick(float DeltaTime) override;

	/**
	* @return Getter for Destination Location
	*/
	FVector GetDestinationLocation() const;

	/**
	* @param NewLocation Setter for the DestinationLocation
	*/
	void SetDestinationLocation(FVector NewLocation);

	/**
	*Set the Destination Location to FVector(0)
	*/
	void EmptyDestinationLocation();

	/**
	* Checks to see if the Destination Vector Is 0
	* @return True if the destination is 0 and false if it isnt
	*/
	bool CheckDestinationLocation() const;

	/**
	* @return Returns the patrolling path index getter
	*/
	int GetPatrollingPathIndex() const;

	/**
	* Increment the path index by one and modulo it by the number of point in the path to ensure that it isnt out of range
	*/
	void IncrementPatrollingPathIndex();

	/**
	 * @return Getter for Explosion Radius
	 */
	float GetExplosionRadius();
	
	/**
	* @return Getter the Suicider Attacking Enum
	*/
	ESuiciderAttackEnums GetSuiciderAttackingState();

	/**
	* Used Queuing it destroy pattern
	*/
	bool bNeedDestroyed = false;

	/**
	* @return Getter for Patrol Point Radius Tolerance
	*/
	float GetPatrolPointRadiusTolerance() const;

	

	/**
	* @return Getter for MovingTimer
	*/
	float GetMovingTimer();
	

	
private:
	/**
	* Nav Volume tolerance
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Suicider", meta = (AllowPrivateAccess = "true"))
	float PatrolPointRadiusTolerance = 200;
	
	/**
	* Amount of time the enemy has to be sitting still to try and get a new location
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Suicider", meta = (AllowPrivateAccess = "true"))
	float MovingTimer = 2.0f;

	
	
	/**
	 * Location the Suicider is try to get to
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Suicider", meta = (AllowPrivateAccess = "true"))
	FVector DestinationLocation = FVector(0);

	/**
	 * Radius after the slime blows up that will be affected by the slime
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Suicider", meta = (AllowPrivateAccess = "true"))
	float ExplosionRadius;
	
	/**
	 * Suicider Attack Enum to determine what type of attacks it will have
	 * For Later Implementation
	 * Will Default to red which is just big damage
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Suicider", meta = (AllowPrivateAccess = "true"))
	ESuiciderAttackEnums SuiciderAttackState;
	/**
	 * Patrolling path index that is internally kept for the engineer to know where in the patrol it is heading towards
	 */
	int PatrollingPathIndex = 0;
};
