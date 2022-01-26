// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponTest/WeaponSystem/ProjectileParent.h"

#include "AIEnemyParent.h"
#include "BlastTestCharacter.h"
#include "DrawDebugHelpers.h"

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
			Cast<ABlastTestCharacter>(GetInstigator())->OnProjectileHitEnemy();
		
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString("Hit Enemy"));
		
		AAIEnemyParent* Enemy = Cast<AAIEnemyParent>(OtherActor);
		if (Enemy)
		{
			Enemy->DealDamageToEnemy(DamageAmount);
		}		
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

