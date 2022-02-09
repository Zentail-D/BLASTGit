// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponTest/WeaponSystem/RocketProjectile.h"

#include "AIEnemyParent.h"
#include "BlastTestCharacter.h"
#include "DrawDebugHelpers.h"
#include "NetworkChar.h"
//#include "Kismet/KismetSystemLibrary.h"


ARocketProjectile::ARocketProjectile()
{
	if (!SphereCollider)
	{
		SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collider"));
		SphereCollider->SetupAttachment(RootComponent); 
	}
	if (CollisionComponent)
	{
		CollisionComponent->OnComponentBeginOverlap.Clear();
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ARocketProjectile::OnOverlap);
	}
}

void ARocketProjectile::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority())	// if we arent the server then dont continue
		return;
	if (!OwningPlayer)		// make sure we have our owning player set
		{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString("Projectile Owning Player not set!"));
		return;
		}
	if (OtherActor->Tags.Contains("Player") && OtherActor->GetName() == GetInstigator()->GetName())
	{
		return;	// we are hitting ourselves when the projectile spawns
	}
	if (!OtherActor->Tags.Contains("Player") && !OtherActor->Tags.Contains("Environment") && !OtherActor->Tags.Contains("Enemy") )
	{
		return; // were overlapping with something we dont care about
	}
	// make sure to also cull out the hit actors we dont care about
	if (OtherActor->Tags.Contains("Projectile") || OtherActor->GetName() == OwnerName)
	{
		return;
	}
	
	//get list of all actors affected by the aoe 
	TArray<AActor*> OverlappedActors;
	SphereCollider->GetOverlappingActors(OverlappedActors);
	if(bDrawExplosion)
	{
		//draw sphere for debugging
		DrawDebugSphere(GetWorld(),SphereCollider->GetComponentLocation()/*-FVector(0,0,100)*/,ExplosionRadius,12,FColor::Red,false,20.0f,SDPG_World,2.0);
	}
	//go through that list and damage all enemies
	for (AActor* OverlappedActor : OverlappedActors)
	{
		
		if (OverlappedActor->Tags.Contains("Enemy"))
		{
			// notify the player that we hit an enemy
			if (GetInstigator())
				Cast<ANetworkChar>(GetInstigator())->OnNotifyProjectileHitEnemy();
			
			AAIEnemyParent* Enemy = Cast<AAIEnemyParent>(OverlappedActor);
			if (Enemy)
			{
				Enemy->DealDamageToEnemy(DamageAmount);
				if(Enemy->GetCurrentHealth()<=0){
					Enemy->GetMesh()->SetSimulatePhysics(true);
					Enemy->GetMesh()->AddRadialImpulse(SphereCollider->GetComponentLocation()-FVector(0,0,100),ExplosionRadius,ExplosionStrength,ERadialImpulseFalloff::RIF_Constant,true);
				}
			}
		}
		if (OtherActor->Tags.Contains("Player"))
		{
			// This is an enemy player
			ANetworkChar* Player = Cast<ANetworkChar>(OtherActor);
			Player->DealDamageToPlayer(DamageAmount);
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, FString("Hit Player"));
		}
	}
	
	// destroy self after dealing with first collision
	CollisionComponent->Deactivate();
	SphereCollider->Deactivate();
	Destroy();
}

void ARocketProjectile::SetExplosionRadius(float NewExplosionRadius)
{
	ExplosionRadius = NewExplosionRadius;
}

float ARocketProjectile::GetExplosionRadius() const
{
	return ExplosionRadius;
}

void ARocketProjectile::SetExplosionStrength(float NewExplosionStrength)
{
	ExplosionStrength = NewExplosionStrength;
}

float ARocketProjectile::GetExplosionStrength() const
{
	return ExplosionStrength;
}

void ARocketProjectile::BeginPlay()
{
	Super::BeginPlay();
	SphereCollider->SetSphereRadius(ExplosionRadius);
}
