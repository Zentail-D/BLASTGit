// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponSystem/RocketProjectile.h"

#include "NetworkChar.h"
#include "Kismet/KismetSystemLibrary.h"


void ARocketProjectile::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	//add object types the aoe will affect to its list
	AffectedObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	//add actors(the player that shot it the aoe will ignore to its list
	ActorsToIgnore.Add(GetOwner());
	//get list of all actors affected by the aoe 
	TArray<AActor*> OverlappedActors;
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(),GetActorLocation(),ExplosionRadius,AffectedObjectTypes,nullptr,ActorsToIgnore,OverlappedActors);
	//go through that list and damage all enemies
	for (AActor* OverlappedActor : OverlappedActors)
	{
		//draw sphere for debugging
		/*
		DrawDebugSphere(GetWorld(),CollisionComp->GetComponentLocation(),ExplosionRadius,12,FColor::Red,false,2.0f,SDPG_World,2.0);
		*/
		//if its an AI
		AAIEnemyParent* EnemyAIInRadius = Cast<AAIEnemyParent>(OverlappedActor);
		if (EnemyAIInRadius)
		{
			EnemyAIInRadius->DealDamageToEnemy(DamageAmount);
		}
		//if its a player
		ANetworkChar* EnemyPlayerInRadius = Cast<ANetworkChar>(OverlappedActor);
		if (EnemyPlayerInRadius)
		{
			EnemyPlayerInRadius->DealDamageToPlayer(DamageAmount);	
		}
	}
}

void ARocketProjectile::SetExplosionRadius(float NewExplosionRadius)
{
	ExplosionRadius = NewExplosionRadius;
}

float ARocketProjectile::GetExplosionRadius() const
{
	return ExplosionRadius;
}
