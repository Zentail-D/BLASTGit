// Fill out your copyright notice in the Description page of Project Settings.


#include "EngineerSpawner.h"

AEngineerSpawner::AEngineerSpawner()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEngineerSpawner::SpawnWave()
{
	if(EnemiesTotal<NumOfEnemies)
	{
		if(EnemyTypes.Num()>0)
		{
			int RandomNum;
			for(int Index = 0; Index < EnemiesPerWave; Index++)
			{
				RandomNum= FMath::RandRange(1,EnemyTypes.Num());
		
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				SpawnParams.Instigator = GetInstigator();

				AAIEnemyParent* EnemyParent = GetWorld()->SpawnActor<AAIEnemyParent>(EnemyTypes[RandomNum-1], SpawnLocation(), FRotator(0,0,0), SpawnParams);

				if(EnemyParent)
				{
					//Set the Player character and the patrol pattern and increment total enemies
					EnemyParent->SetPlayerCharacters(GetPlayerCharacters());
					EnemyParent->SetPatrollingPath(GetPatrollingPath());
					EnemiesTotal++;
				}
		
			}
		}
	}
}

FVector AEngineerSpawner::SpawnLocation() const
{
	float const X = FMath::FRandRange(-1,1);
	float const Y = FMath::FRandRange(-1,1);

	FVector const DesiredLocation = FVector(X,Y,0) * SpawningRadius;
	
	return DesiredLocation + GetActorLocation();
}

void AEngineerSpawner::BeginPlay()
{
	Super::BeginPlay();
	if(!GetPatrollingPath())
	{

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
		APatrollingPath* PatrolPath = GetWorld()->SpawnActor<APatrollingPath>(PatrolClass, GetActorLocation(), FRotator(0,0,0), SpawnParams);
		if(PatrolPath)
		{
			TArray<FVector> NewPatrollingPath;
			FVector const ActorLocation = GetActorLocation();
			//Left
			FVector location = FVector(-PatrollingPathDistanceFromSpawner,0, ActorLocation.Z);
			NewPatrollingPath.Push(location);
			//Left Top
			location = FVector(-PatrollingPathDistanceFromSpawner,-PatrollingPathDistanceFromSpawner, ActorLocation.Z);
			NewPatrollingPath.Push(location);
			//Top
			location = FVector(0,-PatrollingPathDistanceFromSpawner, ActorLocation.Z);
			NewPatrollingPath.Push(location);
			//Top Right
			location = FVector(PatrollingPathDistanceFromSpawner,-PatrollingPathDistanceFromSpawner, ActorLocation.Z);
			NewPatrollingPath.Push(location);
			//Right
			location = FVector(PatrollingPathDistanceFromSpawner,0, ActorLocation.Z);
			NewPatrollingPath.Push(location);
			//Right Lower
			location = FVector(PatrollingPathDistanceFromSpawner,PatrollingPathDistanceFromSpawner, ActorLocation.Z);
			NewPatrollingPath.Push(location);
			//Lower
			location = FVector(0,PatrollingPathDistanceFromSpawner, ActorLocation.Z);
			NewPatrollingPath.Push(location);
			//Lower Left
			location = FVector(-PatrollingPathDistanceFromSpawner,PatrollingPathDistanceFromSpawner, ActorLocation.Z);
			NewPatrollingPath.Push(location);
			PatrolPath->SetPatrolPathPoints(NewPatrollingPath);
			SetPatrollingPath(PatrolPath);
		}
	}
}

void AEngineerSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//Destroy will life time reaches zero
	LifeSpan-=DeltaTime;
	if(LifeSpan<0)
	{
		Destroy();
	}
}

float AEngineerSpawner::GetTimeTillNextSpawn() const
{
	return TimerTillNextSpawn;
}

bool AEngineerSpawner::GetIsActive()
{
	return bActive;
}
