// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZoneSpawner.generated.h"

class AZoneTool;

UCLASS()

class  AZoneSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AZoneSpawner();

	/**The spawners USceneComponent */
	UPROPERTY(VisibleDefaultsOnly)
	USceneComponent* Spawner;

	TArray<TSubclassOf<class AAIEnemyParent>> EnemysToSpawn;
	
	/**Radius from spawner enemies can spawn*/
	float SpawnRadius;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;
	
	void SpawnEnemies( FVector CenterOfZone, FActorSpawnParameters SpawnPara);

	void SetParentZone(AZoneTool* NewParentZone);

private:
	/**A Reference to the Zone the spawner belongs to */
	UPROPERTY()
	AZoneTool* ParentZone;

};
