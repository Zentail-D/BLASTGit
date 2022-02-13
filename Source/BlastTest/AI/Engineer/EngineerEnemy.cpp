// Fill out your copyright notice in the Description page of Project Settings.


#include "EngineerEnemy.h"


// Sets default values
AEngineerEnemy::AEngineerEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void AEngineerEnemy::BeginPlay()
{
	Super::BeginPlay();
	if(!DroneSwarm)
	{
		if(GetWorld())
		{
			DroneSwarm = GetWorld()->SpawnActor<AEngineerDroneSwarm>(DroneSwarmClass,GetActorLocation()+FVector(0,0,250),GetActorRotation());
			if(DroneSwarm)
			{
				
				DroneSwarm->AttachToActor(this,FAttachmentTransformRules::KeepWorldTransform);
			}
		}
	}
	if(DronePatrols.Num()==0)
	{
		if(GetWorld())
		{
			APatrollingPath* DronePatrol = GetWorld()->SpawnActor<APatrollingPath>(PatrolClass,GetActorLocation()+FVector(0,0,250),GetActorRotation());
			if(DronePatrol)
			{
				
				DronePatrol->AttachToActor(this,FAttachmentTransformRules::KeepWorldTransform);
				DronePatrols.Add(DronePatrol);
			}
		}
	}
	else
	{
		for(int Index=0; Index < DronePatrols.Num();Index++)
		{
			DronePatrols[Index]->AttachToActor(this,FAttachmentTransformRules::KeepWorldTransform);
		}
	}
}

void AEngineerEnemy::DestroyChildren()
{
	/*if(DroneSwarm)
	{
		DroneSwarm->Destroy();
	}
	for(int Index=0;Index < DronePatrols.Num();Index++)
	{
		if(DronePatrols[Index])
		{
			DronePatrols[Index]->Destroy();
		}
	}
	DronePatrols.Empty();
	for(int Index=0; Index <SpawnedEngineerTools.Num(); Index++)
	{
	    if(SpawnedEngineerTools[Index])
	    {
		    SpawnedEngineerTools[Index]->SafeToolDestroy();
	    }
	}
	SpawnedEngineerTools.Empty();*/
}

int AEngineerEnemy::GetPatrollingPathIndex() const
{
	return PatrollingPathIndex;
}

void AEngineerEnemy::IncrementPatrollingPathIndex()
{
	PatrollingPathIndex++;
	PatrollingPathIndex%=GetPatrollingPath()->NumOfPoint();
}

TArray<TSubclassOf<AEngineerToolsParent>> AEngineerEnemy::GetEngineerTools() const
{
	return EngineerTools;
}

float AEngineerEnemy::GetSpawningTimer() const
{
	return SpawningTimer;
}

float AEngineerEnemy::GetDroneSpawningDelayTimerPatrolling() const
{
	return DroneSpawningDelayTimerPatrolling;
}

float AEngineerEnemy::GetDroneSpawningDelayTimerPlayer() const
{
	return DroneSpawningDelayTimerPlayer;
}

TSubclassOf<AEngineerToolsParent> AEngineerEnemy::GetDroneClass() const
{
	return DroneClass;
}

APatrollingPath* AEngineerEnemy::GetRandomDronePath()
{
	int const RandomNum = FMath::RandRange(1,DronePatrols.Num());
	return DronePatrols[RandomNum-1];
}

AEngineerDroneSwarm* AEngineerEnemy::GetEngineerDroneSwarm() const
{
	return DroneSwarm;
}

void AEngineerEnemy::AddEngineerToolToArray(AEngineerToolsParent* EngineerTool)
{
	SpawnedEngineerTools.Add(EngineerTool);
}

bool AEngineerEnemy::CanSpawnInDrone() const
{
	return CurrentNumberOfDrones<=MaxNumberOfDrones; 
}

bool AEngineerEnemy::CanSpawnInMine() const
{
	return CurrentNumberOfMines<=MaxNumberOfMines;
}

bool AEngineerEnemy::CanSpawnInTurret() const
{
	return CurrentNumberOfTurrets<=MaxNumberOfTurrets;
}

void AEngineerEnemy::AddOneToCurrentNumberOfDrones() 
{
	CurrentNumberOfDrones++;
}

void AEngineerEnemy::AddOneToCurrentNumberOfMines() 
{
	CurrentNumberOfMines++;
}

void AEngineerEnemy::AddOneToCurrentNumberOfTurret() 
{
	CurrentNumberOfTurrets++;
}

void AEngineerEnemy::SubtractOneToCurrentNumberOfDrones()
{
	CurrentNumberOfDrones--;
}

void AEngineerEnemy::SubtractOneToCurrentNumberOfMines()
{
	CurrentNumberOfMines--;
}

void AEngineerEnemy::SubtractOneToCurrentNumberOfTurret()
{
	CurrentNumberOfTurrets--;
}
