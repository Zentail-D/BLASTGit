// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkChar.h"
#include "EnemyProjectile.h"
#include "InGameUI.h"
#include "InventoryComponent.h"
#include "Kismet/GameplayStatics.h"

void ANetworkChar::ForceRespawnPlayer()
{
	RespawnPlayer();
}

// Sets default values
ANetworkChar::ANetworkChar(const class FObjectInitializer& ObjectInitializer) 
{
	PrimaryActorTick.bCanEverTick = true;


	// get reference to our inventory component
	if(!PlayerInventory)
	{
		PlayerInventory= CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	}
	
		
}

// Called when the game starts or when spawned
void ANetworkChar::BeginPlay()
{
	Super::BeginPlay();	

	TArray<AActor*> NewZoneCon;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),AZoneToolCon::StaticClass(), NewZoneCon);
	//Adding this player to the ZoneToolCons Array of Players so AI spawned from it can target player
	if(NewZoneCon.Num() != 0)
		ZoneCon = Cast<AZoneToolCon>(NewZoneCon[0]);

	if(ZoneCon != nullptr)
		ZoneCon->AddPlayer(this);

}

void ANetworkChar::Destroyed()
{
	Super::Destroyed();
}

bool ANetworkChar::DealDamageToPlayer(float DamageToDeal)
{
	PlayerInventory->PlayerTakeDamage(DamageToDeal);
	return PlayerInventory->GetCurrentPlayerHealth()<=0.f;
}

void ANetworkChar::OnNotifyProjectileHitEnemy()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, FString("Hit"));
}

// Called every frame
void ANetworkChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//GEngine->AddOnScreenDebugMessage(-1,.01,FColor::Red,TEXT("I'm ticking"));
	const float CurHealth = PlayerInventory->GetCurrentPlayerHealth();
	if (HasAuthority())
		GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Blue,  GetName() + FString(": Health = ") + FString::SanitizeFloat(CurHealth));
	if(PlayerInventory->GetCurrentPlayerHealth()<=0)
	{
		//GEngine->AddOnScreenDebugMessage(-1,.4,FColor::Red,TEXT("I'm a dead bitch"));
		// our health has dropped below zero so wer respawn now
		RespawnPlayer();
	}

}

void ANetworkChar::RespawnPlayer()
{
	// find all player start actors in the world
	TArray<AActor*> PlayerStartArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), PlayerStart, PlayerStartArray);

	if (PlayerStartArray.Num() == 0)	// make sure we actually have something in our player start array
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("No player starts found in level"));
		return;
	}
	// select a spawn point at random
	// TODO: i imagine that once we have a larger game flow in place this will need more than just a random index
	const AActor* ChosenSpawn = PlayerStartArray[FMath::FRandRange(0, PlayerStartArray.Num()-1)];

	// reset our health
	PlayerInventory->SetCurrentPlayerHealth(PlayerInventory->GetMaxPlayerHealth());

	// Reset the player weapon back to spawning state
	PlayerInventory->ResetMods();
	
	// set our actor location to the spawn point
	SetActorLocation(ChosenSpawn->GetActorLocation());
}

void ANetworkChar::RemoveFromZone()
{
	ZoneCon->RemovePlayer(this);
}
