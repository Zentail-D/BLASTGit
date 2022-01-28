// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponTest/WeaponSystem/RailgunProjectile.h"

#include "AIEnemyParent.h"
#include "BlastTestCharacter.h"
#include "NetworkChar.h"

ARailgunProjectile::ARailgunProjectile()
{
	if (CollisionComponent)
	{
		CollisionComponent->OnComponentBeginOverlap.Clear();
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ARailgunProjectile::OnOverlap);
	}
}

void ARailgunProjectile::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//if overlap with environment
	if(OtherActor->Tags.Contains("Environment"))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString("Hit Non-Enemy"));
		Destroy();
	}

	//if overlap with enemy
	if(OtherActor->Tags.Contains("Enemy"))
	{
		// notify the player that we hit an enemy
		if (GetInstigator())
			Cast<ANetworkChar>(GetInstigator())->OnNotifyProjectileHitEnemy();
		
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString("Hit"));
		
		AAIEnemyParent* Enemy = Cast<AAIEnemyParent>(OtherActor);
		if (Enemy)
		{
			Enemy->DealDamageToEnemy(DamageAmount);
		}		
	}
}

void ARailgunProjectile::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ARailgunProjectile::BeginPlay()
{
	Super::BeginPlay();
}
