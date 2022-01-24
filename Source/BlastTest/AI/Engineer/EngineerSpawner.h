// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EngineerToolsParent.h"
#include "EngineerSpawner.generated.h"

/**
 * 
 */
UCLASS()
class  AEngineerSpawner : public AEngineerToolsParent
{
	GENERATED_BODY()

public:	
	/**
	* Enable the tick for the drone swarm
	*/
	AEngineerSpawner();

	/**
	 * Spawn in a wave of enemies. Uses NumPerWave to determine how many to spawn. Will not spawn more than more than max enemies and will spawn random enemies in EnemyType
	 */
	void SpawnWave();

	/**
	*Checks to see if there is a patrol path assigned to it if there isnt it will create it own
	*Called when the game starts or when spawned
	*/
	virtual void BeginPlay() override;

	/**
	*Called every frame
	* Checks to see if it is pass it's life time and will destroy if needed
	*/
	virtual void Tick(float DeltaTime) override;

	/**
	 * @return Getter for Time Till Next Spawn
	 */
	float GetTimeTillNextSpawn() const;

	/**
	 * @return Getter for bActive
	 */
	bool GetIsActive();
	
private:
	/**
	 *Helper function for spawn wave
	 * @return Returns a FVector of a random spot around the Spawn to have the wave spawn to
	 */
	FVector SpawnLocation() const;
	
	/**Array af classes of enemies*/
	UPROPERTY(EditAnywhere, Category = "AI Settings | General Stats", meta = (AllowPrivateAccess = true))
	TArray<TSubclassOf<AAIEnemyParent>> EnemyTypes;
	
	/** The total amount of enemies this spawner currently has spawned */
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category= "AI Settings | General Stats", meta = (AllowPrivateAccess = true))
	float EnemiesTotal =0;

	/**The class of patrol path so that it can spawn a patrol path in if there isnt one set*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category= "AI Settings | General Stats", meta = (AllowPrivateAccess = true))
	UClass* PatrolClass;

	/**Max Number of Enemies the Spawner can spawn*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category= "AI Settings | General Stats", meta = (AllowPrivateAccess = true))
	int NumOfEnemies = 4;
	
	/** How many enemies will spawn per wave of enemies*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category= "AI Settings | General Stats", meta = (AllowPrivateAccess = true))
	int EnemiesPerWave = 2;

	/**Timer to check if spawning is necessary*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category= "AI Settings | General Stats", meta = (AllowPrivateAccess = true))
	float TimerTillNextSpawn = 3;

	/**Radius around the spawner to spawn the enemies*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category= "AI Settings | General Stats", meta = (AllowPrivateAccess = true))
	float SpawningRadius =500.0f;

	/**How far from the spawner the patrolling path will be created*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category= "AI Settings | General Stats", meta = (AllowPrivateAccess = true))
	float PatrollingPathDistanceFromSpawner =200.0f;

	/**IF active is true then the spawner will attempt to spawn enemies if it is false then it won't try*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category= "AI Settings | General Stats", meta = (AllowPrivateAccess = true))
	bool bActive =true;

	/**How long the spawner will stay alive before destroying itself*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category= "AI Settings | General Stats", meta = (AllowPrivateAccess = true))
	float LifeSpan =6.0f;

};
