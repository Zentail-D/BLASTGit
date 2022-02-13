// Fill out your copyright notice in the Description page of Project Settings.


#include "ZoneToolCon.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AZoneToolCon::AZoneToolCon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MaxEnemyCount = 20;
	CurrEnemyCount = 0;
	MaxEnemyPerWave = 2;
	MinEnemyPerWave = 1;
	DespawnAmount = -1;
	GenMaxNum = 0;

}

// Called when the game starts or when spawned
void AZoneToolCon::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*> newZones;
	TArray<AActor*> Players;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),AZoneTool::StaticClass(), newZones);
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(),ACharacter::StaticClass(), Players);

	//places all the zones in the world into the zone array
	for(int32 i=0; i<newZones.Num();i++)
	{
		Zones.Add(Cast<AZoneTool>(newZones[i]));
		Zones[i]->MaxEnemyCount=MaxEnemyCount/newZones.Num();
		Zones[i]->ZoneControler = this;
	}

	for (int32 i = 0; i < Zones.Num(); i++)
	{
		AssignNeighborZones(i,2);
	}

	this->SpawnGenrators(GenMaxNum);
}

void AZoneToolCon::SetMaxEnemyCount(int32 NewMaxEnemyCount)
{
	MaxEnemyCount = NewMaxEnemyCount;
	//Sets the maxEnemyCount for each of the zones in the world
	for(int32 i=0; i<Zones.Num();i++)
	{
		Zones[i]->MaxEnemyCount=MaxEnemyCount/Zones.Num();
	}
	
}

// Called every frame
void AZoneToolCon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CurrEnemyCount = 0;
	NumPlayersInZones = 0;
	for(int32 i=0; i<Zones.Num();i++)
	{
		NumPlayersInZones += Zones[i]->PlayerInZone.Num();
		CurrEnemyCount += Zones[i]->EnemiesInZone.Num();
		if(Zones[i]->bIsPlayerInZone==true)
		{
			Zones[i]->bIsActive = true;
			Zones[i]->SetNeighborsActive();
		}	
	}
	
}

void AZoneToolCon::AssignNeighborZones(int32 ZoneIndex,int32 NeighborCount )
{	
	
	if(NeighborCount<=Zones.Num()-1)
	{
		float NearstDistance;
		AZoneTool* NearstZone;
		for(int j = 0; NeighborCount>j;j++)
		{
			NearstDistance = 100000000.0f;
			for (int32 i = 0; i < Zones.Num(); i++)
			{
				
				float tempDistance = FVector::Dist(Zones[ZoneIndex]->GetActorLocation(),Zones[i]->GetActorLocation());
				
				if(ZoneIndex!=i && false==Zones[ZoneIndex]->NeighborsZones.Contains(Zones[i]))
				{
					if(tempDistance < NearstDistance)
					{
						NearstDistance = tempDistance;
						NearstZone = Zones[i];
					}
				}
				
			}
			Zones[ZoneIndex]->NeighborsZones.Add(NearstZone);
		}
	}
}

void AZoneToolCon::RemovePlayer(ACharacter* OldPlayer)
{
	if(HasAuthority())
	{
		if(OldPlayer != nullptr)
		{
			PlayersInWorld.Remove(OldPlayer);
			for(int32 i=0; i<Zones.Num();i++)
			{
				for(int32 j=0;j<Zones[i]->EnemiesInZone.Num();j++)
				{
					//Zones[i]->EnemiesInZone[i]->SetPlayerCharacters(PlayersInWorld);
				}
			}
		}
	}
}

void AZoneToolCon::AddPlayer(ACharacter* NewPlayer)
{
	if(HasAuthority())
		if(NewPlayer != nullptr)
		{
			PlayersInWorld.Add(NewPlayer);
		}
}

void AZoneToolCon::SpawnGenrators(int GenNum)
{
	TArray<FVector> AllGenLocations;
	for(int i = 0; i <= Zones.Num()-1;i++)
	{
		for(int j = 0; j<= Zones[i]->GenLocations.Num()-1;j++)
		{
			AllGenLocations.Add(Zones[i]->GetActorLocation()+Zones[i]->GenLocations[j]);
		}
	}
	if(GenNum <= AllGenLocations.Num())
	{
		while(GenNum != 0)
		{
			int GenLo = FMath::RandRange(0,AllGenLocations.Num()-1);
			FVector Location = AllGenLocations[GenLo];
			AllGenLocations.RemoveAt(GenLo);
			FActorSpawnParameters SpawnPara;
			SpawnPara.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			AGenerator* temp = GetWorld()->SpawnActor<AGenerator>(GenClass, Location,FRotator(0,0,0),SpawnPara);
			temp->SetCanActive(true);//Lets player interact with the generator
			GensInWorld.Add(temp);
			GEngine->AddOnScreenDebugMessage(-1,10.0f,FColor::Red,TEXT("Spawn Gen"));
			GenNum=  GenNum-1;
		}
	}
	
}


