// Fill out your copyright notice in the Description page of Project Settings.


#include "ZoneSpawner.h"
#include"ZoneToolCon.h"
#include "ZoneTool.h"
#include "AI/Engineer/EngineerEnemy.h"
#include "AI/Flying/FlyingEnemyAI.h"
#include "AI/Grunt/GruntAIEnemy.h"
#include "AI/Suicider/SuiciderEnemy.h"
#include "GameFramework/Character.h"

// Sets default values
AZoneSpawner::AZoneSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	Spawner = CreateDefaultSubobject<USceneComponent>(TEXT("Spawner"));

	RootComponent = Spawner;

	SpawnRadius = 0.0f;

}

// Called when the game starts or when spawned
void AZoneSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

void AZoneSpawner::SpawnEnemies(FVector CenterOfZone, FActorSpawnParameters SpawnPara)
{
	if(GetLocalRole() > ROLE_AutonomousProxy)
	{
		if(EnemysToSpawn.Num() !=0)
		{
			int32 RandEnemy = FMath::RandRange(0,EnemysToSpawn.Num()-1);
			TSubclassOf<AAIEnemyParent> SpawnClass = EnemysToSpawn[RandEnemy];
			AAIEnemyParent* newEnemy;
			if(SpawnClass != nullptr)
			{
				//get the location
				FVector Location = CenterOfZone + this->GetActorLocation();
				Location.X = FMath::RandRange((Location.X-SpawnRadius),(Location.X+SpawnRadius));
				Location.Y = FMath::RandRange((Location.Y-SpawnRadius),(Location.Y+SpawnRadius));
				//spawning the given enemy at the spawners location
				newEnemy =GetWorld()->SpawnActor<AAIEnemyParent>(SpawnClass, Location,FRotator(0,0,0),SpawnPara);
				//checking to see if Actor was spawned
				if(newEnemy != nullptr)
				{
					newEnemy->SetPlayerCharacters(ParentZone->ZoneControler->PlayersInWorld);
					//checks what class is being spawned so we can give it the proper Patrol paths(once the other classes are ready do the same for them) 
					if(newEnemy->IsA(AFlyingEnemyAI::StaticClass()))
					{
						//cast to Flying AI so i can set the Zone3DNav
						Cast<AFlyingEnemyAI>(newEnemy)->SetNavigationVolume(ParentZone->Zone3DNav);
						//Need to scale the AI so it can use the nav properly
						//newEnemy->SetActorScale3D(FVector(0.25f,0.25f,0.25f));
						newEnemy->SetPatrollingPath(ParentZone->FlyingPatrolPaths[FMath::RandRange(0,ParentZone->FlyingPatrolPaths.Num()-1)]);
					}
					else if(newEnemy->IsA(AGruntAIEnemy::StaticClass()))
					{
						newEnemy->SetPatrollingPath(ParentZone->GruntPatrolPaths[FMath::RandRange(0,ParentZone->GruntPatrolPaths.Num()-1)]);
					}
					else if(newEnemy->IsA(AEngineerEnemy::StaticClass()))
					{
						newEnemy->SetPatrollingPath(ParentZone->EngineerPatrolPaths[FMath::RandRange(0,ParentZone->EngineerPatrolPaths.Num()-1)]);
					}
					else
					{
						newEnemy->SetPatrollingPath(ParentZone->SuiciderPatrolPaths[FMath::RandRange(0,ParentZone->SuiciderPatrolPaths.Num()-1)]);
					}
					ParentZone->EnemiesInZone.Add(newEnemy);
				}
			}
		}
	}
}

void AZoneSpawner::SetParentZone(AZoneTool* NewParentZone)
{
	ParentZone = NewParentZone;
}
