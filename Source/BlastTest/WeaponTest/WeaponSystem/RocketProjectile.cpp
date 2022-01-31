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
				//Enemy->GetMesh()->SetSimulatePhysics(true);
				
				
				Enemy->DealDamageToEnemy(DamageAmount);
				if(Enemy->GetCurrentHealth()==0)
				{
					//Enemy->GetMesh()->SetSimulatePhysics(true);
				
					//Enemy->GetMesh()->AddImpulse((Enemy->GetActorLocation()-SphereCollider->GetComponentLocation())*ExplosionStrength);
					Enemy->GetMesh()->AddRadialImpulse(SphereCollider->GetComponentLocation()-FVector(0,0,100),ExplosionRadius,ExplosionStrength,ERadialImpulseFalloff::RIF_Constant,true);
				}
			}
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
