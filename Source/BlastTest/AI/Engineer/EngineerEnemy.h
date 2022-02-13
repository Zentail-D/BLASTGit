// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIEnemyParent.h"
#include "EngineerDroneSwarm.h"
#include "EngineerToolsParent.h"
#include "GameFramework/Character.h"
#include "EngineerEnemy.generated.h"


UCLASS()
class  AEngineerEnemy : public AAIEnemyParent
{
	GENERATED_BODY()

public:
	/**
	* Enable the tick for the drone swarm
	*/
	AEngineerEnemy();

	/**
	* Boolean on whether or not the Engineer is moving
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Engineer", meta = (AllowPrivateAccess = "true"))
	bool bNeedNewPatrolPoint = false;

	/**
	* @return Returns the patrolling path index getter
	*/
	int GetPatrollingPathIndex() const;

	/**
	* Increment the path index by one and modulo it by the number of point in the path to ensure that it isnt out of range
	*/
	void IncrementPatrollingPathIndex();

	/**
	 * @return Reference to all the engineer tools that the are included in the engineer that it can spawn
	 */
	TArray<TSubclassOf<AEngineerToolsParent>> GetEngineerTools() const ;

	/**
	 * @return Getter for Spawning Timer
	 */
	float GetSpawningTimer() const;

	/**
	 * @return Getter for DroneSpawningDelayTimerPatrolling
	 */
	float GetDroneSpawningDelayTimerPatrolling() const;
	
	/**
	* @return Getter for DroneSpawningDelayTimerPlayer
	*/
	float GetDroneSpawningDelayTimerPlayer() const;

	/**
	 * @return Getter for the Drone Class
	 */
	TSubclassOf<AEngineerToolsParent> GetDroneClass() const;

	/**
	 * @return Getter for a Random Drone Path in the Drone Paths Array
	 */
	APatrollingPath* GetRandomDronePath();

	/**
	 * @return Getter for Engineer Drone Swarm
	 */
	AEngineerDroneSwarm* GetEngineerDroneSwarm() const;

	/**
	 * Adds a single engineer tool into the SpawnEngineerTools
	 * @param EngineerTool Reference to the tool that we want to add
	 */
	void AddEngineerToolToArray(AEngineerToolsParent* EngineerTool);

	/**
	 * Check to see if we can spawn in a Tool of a particalar type and if we can return true
	 * @return True if we can spawn in the tool and false if you cant
	 */
	bool CanSpawnInDrone() const;

	/**
	* Check to see if we can spawn in a Tool of a particalar type and if we can return true
	* @return True if we can spawn in the tool and false if you cant
	*/
	bool CanSpawnInMine() const;

	/**
	* Check to see if we can spawn in a Tool of a particalar type and if we can return true
	* @return True if we can spawn in the tool and false if you cant
	*/
	bool CanSpawnInTurret() const;

	/**
	 * Add one to the current Number of Drones
	 */
	void AddOneToCurrentNumberOfDrones();

	/**
	* Add one to the current Number of Mine
	*/
	void AddOneToCurrentNumberOfMines();

	/**
	* Add one to the current Number of Turret
	*/
	void AddOneToCurrentNumberOfTurret();

	/**
	* Subtract one to the current Number of Drones
	*/
	void SubtractOneToCurrentNumberOfDrones();

	/**
	* Subtract one to the current Number of Mine
	*/
	void SubtractOneToCurrentNumberOfMines();

	/**
	* Subtract one to the current Number of Turret
	*/
	void SubtractOneToCurrentNumberOfTurret();

	inline int GetDistanceFromPlayer() const {return DistanceFromPlayer;}

	
protected:
	/**
	 *Checks to see if a drone path was set if not it creates one
	 *Checks to see if any drone path were add into the array if there were it will parent the paths to the engineer so that they move with the engineer
	 *Called when the game starts or when spawned
	 */
	virtual void BeginPlay() override;
	
private:
	
	/**Array for all the engineer tools. It has to be a child of the EngineerToolsParent to be able to add to the array. Add it to this array and it will have a chance to spawn*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Engineer", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<AEngineerToolsParent>> EngineerTools;

	/**The Distance that the engineer will attempt to keep from the player when the engineer sees the player*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Engineer", meta = (AllowPrivateAccess = "true"))
	int DistanceFromPlayer = 1500;

	/**
	 * The Swarm that all the drone that are created are added to
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Engineer", meta = (AllowPrivateAccess = "true"))
	AEngineerDroneSwarm* DroneSwarm;

	/**
	 * Class for the drone swarm if the Engineer needs to create a swarm in begin play
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Engineer", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AEngineerDroneSwarm> DroneSwarmClass;

	/**
	 * Class for the drones when they are created referenced to the blueprint
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Engineer", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AEngineerToolsParent> DroneClass;
	
	/**Array for all the drone patrols that a drone can randomly be assigned to*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Engineer", meta = (AllowPrivateAccess = "true"))
   	TArray<APatrollingPath*> DronePatrols;

	/**
	 * Array of all the tools and drones that the engineer has spawned in
	 */
	TArray<AEngineerToolsParent*> SpawnedEngineerTools;

	/**
	 * Patrol Class for the drone if we need to create a potrol so that the drone can follow something linked to a blueprint
	 */
   	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Engineer", meta = (AllowPrivateAccess = "true"))
   	TSubclassOf<APatrollingPath> PatrolClass;

	/**
	 * Timer for the spawning of tools for the Engineer. 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Engineer", meta = (AllowPrivateAccess = "true"))
	float SpawningTimer = 5;

	/**
	 * Timer for spawning in a new drone while patrolling
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Engineer", meta = (AllowPrivateAccess = "true"))
	float DroneSpawningDelayTimerPatrolling = 10;

	/**
	* Timer for spawning in a new drone while Player Found
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Engineer", meta = (AllowPrivateAccess = "true"))
	float DroneSpawningDelayTimerPlayer = 10;

	/**
	* Max Number of Drones the Engineer Can Spawn
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Engineer", meta = (AllowPrivateAccess = "true"))
	float MaxNumberOfDrones = 15;

	/**
	* Max Number of Mines the Engineer Can Spawn
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Engineer", meta = (AllowPrivateAccess = "true"))
	float MaxNumberOfMines = 5;

	/**
	* Max Number of Turrets the Engineer Can Spawn
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Engineer", meta = (AllowPrivateAccess = "true"))
	float MaxNumberOfTurrets = 5;

	/**
	* Current Number of Drones the Engineer has
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Engineer", meta = (AllowPrivateAccess = "true"))
	float CurrentNumberOfDrones = 0;

	/**
	* Current Number of Mines the Engineer has
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Engineer", meta = (AllowPrivateAccess = "true"))
	float CurrentNumberOfMines = 0;

	/**
	* Current Number of Turrets the Engineer has
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Engineer", meta = (AllowPrivateAccess = "true"))
	float CurrentNumberOfTurrets = 0;

	/**
	 *Informing the Drone swarm and Patrol path that the engineer destroyed itself so they need to die as well
	 */
	void DestroyChildren() override;
	
	/**
	 * Patrolling path index that is internally kept for the engineer to know where in the patrol it is heading towards
	 */
	int PatrollingPathIndex = 0;

};
