// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Generator.generated.h"

UCLASS()
class BLASTTEST_API AGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGenerator();

	/**Time it takes to spawn SpawnObject after being set active**/
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float TimeToGenerate;

	/**Keeping track of till spawning SpawnObject **/
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float CurrentTime;

	/**boolean if set true will spawn SpawnObject after TimeToGenerate time passes**/
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	bool IsActive;

	/**boolean if set true if player can interact with object **/
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	bool CanSetActive;

    /**The class the genrator will spawn**/
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> SpawnObject;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	/** sets the generator to active to build point object
	 @param ActiveParma boolean to set the active statis of the generator
	 ***/
	UFUNCTION(BlueprintCallable)
	void SetActive(bool ActiveParma);

	/** lets player interact with gen 
	@param ActiveParma boolean to set the active status of the generator
	***/
	UFUNCTION(BlueprintCallable)
	void SetCanActive(bool ActiveParma);
	
	/** spawns the ScoreObject**/
	UFUNCTION(BlueprintCallable)
	void SpawnScoreObject();
	
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
