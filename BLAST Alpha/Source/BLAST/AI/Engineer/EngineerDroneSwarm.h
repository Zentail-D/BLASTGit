// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EngineerDrones.h"
#include "AIEnemyParent.h"
#include "EngineerDroneSwarm.generated.h"

UCLASS()
class BLAST_API AEngineerDroneSwarm : public AAIEnemyParent
{
	GENERATED_BODY()

public:
	/**
	 * Enable the tick for the drone swarm
	 */
	AEngineerDroneSwarm();

	/**
	 * @return Getter for all the Drones Reference in this Swarm
	 */
	TArray<class AEngineerDrones*> GetDroneSwarm() const;

	/**
	 *Adds a drone to the swarm
	 * @param Drone The reference to the drone that is wanted to be added to the swarm
	 */
	void AddNewDrone(AEngineerDrones* Drone);

	/**
	 * Remove a specific drone from the swarm
	 * @param Index the index to the drone that is wanted to be removed
	 */
	void RemoveDroneAtIndex(int Index);

	/**
	 * Empty the Drone Swarm of all the drone in it's reference
	 */
	void EmptyDrones();

	/**
	 * Find a out if the location wanted can be used for the drone and if it can then the drone will find a location that it can recursively
	 * @param Destination Location Wanting to test for the Drone Swarm and if the drone can move there
	 * @param BoxExtent How big the area every drone claims
	 * @param ObjectTypes The object types to test off of for what is overlapping the drone location
	 * @param ActorClassFilter The class to filter all collision off of. Normally actor
	 * @return Return FVector to the location that the drone can move to
	 */
	FVector PointOverlappingDrones(FVector Destination, FVector BoxExtent, TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, UClass* ActorClassFilter);
	

private:
	/**
	 * All the drone references for checking if a drone can move to a location
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Drone", meta = (AllowPrivateAccess = "true"))
	TArray<AEngineerDrones*> DroneSwarm;
	
};
