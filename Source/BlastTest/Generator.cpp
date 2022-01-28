// Fill out your copyright notice in the Description page of Project Settings.


#include "Generator.h"

#include "Actor.h"

// Sets default values
AGenerator::AGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TimeToGenerate = 10.0;
	CurrentTime = 0.0;	
	IsActive = false;
	SpawnObject = nullptr;
}

// Called when the game starts or when spawned
void AGenerator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(IsActive)
	{
		CurrentTime += DeltaTime;
		if(CurrentTime >= TimeToGenerate)
		{
			SpawnScoreObject();
			IsActive = false;
			CurrentTime = 0;
		}
	}
}

void AGenerator::SetActive(bool ActiveParma)
{
	IsActive = ActiveParma;
}

void AGenerator::SpawnScoreObject()
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, FString::Printf(TEXT("SpawnScoreObject")));
	
	FActorSpawnParameters SpawnPara;
	SpawnPara.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FVector Location = this->GetActorLocation();
	Location.X += 100; 
	GetWorld()->SpawnActor<AActor>(SpawnObject, Location,FRotator(0,0,0),SpawnPara);
}


