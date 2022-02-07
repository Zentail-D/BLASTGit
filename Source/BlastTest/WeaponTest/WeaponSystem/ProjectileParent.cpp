// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponTest/WeaponSystem/ProjectileParent.h"

#include "AIEnemyParent.h"
#include "BlastTestCharacter.h"
#include "DrawDebugHelpers.h"
#include "NetworkChar.h"

// Sets default values
AProjectileParent::AProjectileParent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	if (!CollisionComponent)
	{
		// Use a sphere as a simple collision representation.
		CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectileParent::OnOverlap);
		CollisionComponent->SetBoxExtent(FVector(5,5,5));
		RootComponent = CollisionComponent;
	}

	if (!ProjectileMovementComponent)
	{
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
		//ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
	}
	CollisionComponent->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComponent->CanCharacterStepUpOn = ECB_No;
	/**replication of bullets*/
	bReplicates = true;
}

void AProjectileParent::FireInDirection(const FVector& ShootDirection) const
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

void AProjectileParent::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Tags.Contains("Player") && OtherActor->GetName() == GetInstigator()->GetName())
	{
		return;	// we are hitting ourselves when the projectile spawns
	}
	if (!OtherActor->Tags.Contains("Player") && !OtherActor->Tags.Contains("Environment") && !OtherActor->Tags.Contains("Enemy") )
	{
		return; // were overlapping with something we dont care about
	}
	
	//if overlap with environment
	if(OtherActor->Tags.Contains("Environment"))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString("Hit Non-Enemy"));
		Destroy();
	}
	
	// notify the player that we hit an enemy
	if (GetInstigator())
		Cast<ANetworkChar>(GetInstigator())->OnNotifyProjectileHitEnemy();
	
	if (OtherActor->Tags.Contains("Enemy"))
	{
		// This is just our regular enemies
		AAIEnemyParent* Enemy = Cast<AAIEnemyParent>(OtherActor);
		if (Enemy)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString("Hit Enemy"));
			Enemy->DealDamageToEnemy(DamageAmount);
			if(Enemy->GetCurrentHealth()<=0)
			{
				Enemy->GetMesh()->SetSimulatePhysics(true);
				Enemy->GetMesh()->AddImpulse(FVector(ProjectileMovementComponent->Velocity*ImpulsePower));
			}
		}
		Destroy();
	}
	if (OtherActor->Tags.Contains("Player"))
	{
		// This is an enemy player
		ANetworkChar* Player = Cast<ANetworkChar>(OtherActor);
		Player->DealDamageToPlayer(DamageAmount);
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, FString("Hit Player"));
		Destroy();
	}
	
}

void AProjectileParent::SetDamageAmount(const int NewDamageAmount)
{
	DamageAmount=NewDamageAmount;
}

int AProjectileParent::GetDamageAmount() const
{
	return DamageAmount;
}

void AProjectileParent::SetImpulsePower(int NewImpulsePower)
{
	ImpulsePower = NewImpulsePower;
}

int AProjectileParent::GetImpulsePower() const
{
	return ImpulsePower;
}

// Called every frame
void AProjectileParent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ProjectileLifespan-=DeltaTime;
	if(ProjectileLifespan<0)
	{
		Destroy();
	}
	if(bShowDebugBox)
	{
		DrawDebugBox(GetWorld(),GetActorLocation(),CollisionComponent->GetScaledBoxExtent(),FColor::Green,false,0.1,0,3);
	}
}

