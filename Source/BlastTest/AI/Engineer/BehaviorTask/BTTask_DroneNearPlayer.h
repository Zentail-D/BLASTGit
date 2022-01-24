// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyControllerParent.h"
#include "EngineerDrones.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_DroneNearPlayer.generated.h"

/**
 * 
 */
UCLASS()
class  UBTTask_DroneNearPlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:

	/**
	* Set to notify on tick so that TickTask is called every frame. And sets the Node Name
	*/
	UBTTask_DroneNearPlayer(FObjectInitializer const& object_initializer);
	/**
	* Find a location near the player to move toward
	* @param OwnerComp Get Reference to Current Enemy and Controller
	* @param NodeMemory Needed to override the Function
	*/
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	/**
	* Move to the location and finish the node when you get there
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
	 * Blackboard  Key that tell the Drone the center location to patrol around
	 */
	UPROPERTY(EditAnywhere, Category = Blackboard, meta = (AllowPrivateAccess = "true"))
	struct FBlackboardKeySelector DroneInRangeLocation;

	/**
	* Blackboard Key that tell the Drone that it is in range for the player it can only be set true in this task to ensure that it will only be in range when it is close to the player can shoot
	*/
	UPROPERTY(EditAnywhere, Category = Blackboard, meta = (AllowPrivateAccess = "true"))
	struct FBlackboardKeySelector InRange;

	/**
	 * The Distance from the player we want the drone to find a location at
	 */
	UPROPERTY(EditAnywhere, Category = Blackboard, meta = (AllowPrivateAccess = "true"))
	float DistanceFromPlayer;

	/**
	 * The Object type we want to check in our box trace normally will be set to Dynamic and Static
	 */
	UPROPERTY(EditAnywhere, Category = Blackboard, meta = (AllowPrivateAccess = "true"))
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes; 

	/**
	 * The Filter to be checking when doing the box trace normally set to Actor
	 */
	UPROPERTY(EditAnywhere, Category = Blackboard, meta = (AllowPrivateAccess = "true"))
	UClass* ActorClassFilter;

	/**
	 * @return Return the Vector Near the player we want to target
	 */
	FVector FindLocationNearPlayer();

	/**
	 * Helper function for  Find Location Near Player
	 * @return return the player index for the player we want to find a location near
	 */
	int FindClosestPlayer() const;

	/**
	 * Helper function for the Find Location Near Player
	 * @return Return the actual vector to the place we want the drone to move to
	 * @param Distance How far from the player we want to find a position
	 * @param PlayerIndex The player index that we want to find a position for
	 */
	FVector VectorWithDistanceFromPlayer(float Distance, int PlayerIndex)  const;

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
	* This is the Distance from the end point when we enter the node used in calculating the force vector;
	*/
	float InitialFinalDistance;
	

	
};
