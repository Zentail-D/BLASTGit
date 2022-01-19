// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EngineerToolsParent.h"
#include "Components/BoxComponent.h"
#include "EngineerDrones.generated.h"

/**
 * 
 */
UCLASS()
class BLAST_API AEngineerDrones : public AEngineerToolsParent
{
	GENERATED_BODY()

public:
	/**
	 * Initializing the mine trigger boxes
	 */
	AEngineerDrones();

	/**
	 * @return Returns the patrolling path index getter
	 */
	int GetPatrollingPathIndex() const;

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
	 * Increment the path index by one and modulo it by the number of point in the path to ensure that it isnt out of range
	 */
	void IncrementPatrollingPathIndex();

	/**
	 * @return Getter for the Rotation Alpha
	 */
	float GetRotationAlpha() const;

	/**
	 * @return Getter for the Acceleration
	 */
	float GetAcceleration() const;


	/**
	 * Runs after the constuctor but before the tick
	 */
	virtual void BeginPlay() override;

	/**
	 * Trigger Box for the drones to tell them if they are in another drones area
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = BoxComponent)
	UBoxComponent* BoxTriggerBox;

	/**
	 * @return Getter from the DroneSwarm Attached to the drone
	 */
	class AEngineerDroneSwarm* GetDroneSwarm() const;

	/**
	 * @param ADroneSwarm Setter for the DroneSwarm
	 */
	void SetDroneSwarm(AEngineerDroneSwarm* ADroneSwarm);

	/**
	* Used Queuing it destroy pattern
	*/
	bool bNeedDestroyed = false;

	/**
	*Called every frame
	* Checks to see if it needs to be destroy and will Destroy it unless it bNeverDestroy is true
	*/
	virtual void Tick(float DeltaSeconds) override;

	/**
	* Overriding function to call when a tool needs to be destroyed
	*/
	void SafeToolDestroy() override;

	/**
	* Used to start the destroy timer
	*/
	bool bStartDestroyTimer = false;

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Engineer Settings", meta = (AllowPrivateAccess = "true"))
	bool bDestroyItselfOnEngineerDeath=true;

	
	/**
	 * PatrollingPathIndex keep internally to inform the task what path index this drone is at
	 */
	int PatrollingPathIndex = 0;

	/**
	 * The location that the drone is heading towards at this current moment
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Drone", meta = (AllowPrivateAccess = "true"))
	FVector DestinationLocation = FVector(0);

	/**
	 * Speed at which it will rotate at
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Movement Settings", meta = (AllowPrivateAccess = "true"))
	float RotationAlpha;

	/**
	 * The number multipled by the Force Vector created in the move tasks that will tell how fast the drone moves
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Movement Settings", meta = (AllowPrivateAccess = "true"))
	float Acceleration;

	/**
	 * Friction for rotation
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Movement Settings", meta = (AllowPrivateAccess = "true"))
	float AngularDamping;

	/**
	 * Friction for movement
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Movement Settings", meta = (AllowPrivateAccess = "true"))
	float LinearDamping;

	/**
	 * Reference to the controlling drone swarm for this drone
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Drone", meta = (AllowPrivateAccess = "true"))
	AEngineerDroneSwarm* DroneSwarm;

	/**
	* Sets the life time the mine has when set to destroy itself
	* how long the drone survives after engineer dies
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Mine Settings", meta = (AllowPrivateAccess = "true"))
	float DroneLifeTime= 5;

	private:
	/**
	* Inform the engineer that it has died
	*/
	void DestroyChildren() override;
};
