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

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float TimeToGenerate;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float CurrentTime;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	bool IsActive;

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

	/** spawns the ScoreObject when called**/
	UFUNCTION(BlueprintCallable)
	void SpawnScoreObject();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
