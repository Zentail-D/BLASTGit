// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyProjectile.h"

#include "NetworkChar.h"

// Sets default values
AEnemyProjectile::AEnemyProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InitialLifeSpan = 15.0f;
	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
	}
	if (!CollisionComponent)
	{
		// Use a sphere as a simple collision representation.
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		CollisionComponent->OnComponentHit.AddDynamic(this, &AEnemyProjectile::OnHit);
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemyProjectile::OnOverlap);
		CollisionComponent->ComponentTags.Add("projectile");
		// Set the sphere's collision radius.
		CollisionComponent->InitSphereRadius(15.0f);
		// Set the root component to be the collision component.
		RootComponent = CollisionComponent;
	}

	if (!ProjectileMovementComponent)
	{
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
		ProjectileMovementComponent->InitialSpeed = 3000.0;
		ProjectileMovementComponent->MaxSpeed = 3000.0;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;


	}
	/**replication of bullets*/
	bReplicates = true;
}

// Called when the game starts or when spawned
void AEnemyProjectile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEnemyProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//Destroy itself if it is too old
	InitialLifeSpan-=DeltaTime;
	if(InitialLifeSpan<0)
	{
		Destroy();
	}

}

void AEnemyProjectile::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

void AEnemyProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	/**if hit enemy Player*/
	ANetworkChar* enemyPlayer = Cast<ANetworkChar>(OtherActor);
	if (enemyPlayer)
	{
		if(enemyPlayer!=GetOwner())
		{
			/**deal damage to enemy player*/
			enemyPlayer->DealDamageToPlayer(DamageAmount);
			/**prep for deletion*/
			Destroy();
		}
	}
}

void AEnemyProjectile::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	/**if overlap enemy Player*/
	ANetworkChar* enemyPlayer = Cast<ANetworkChar>(OtherActor);
	if (enemyPlayer)
	{
		/**deal damage to enemy player	*/
		if(enemyPlayer!=GetInstigator())
		{
			enemyPlayer->DealDamageToPlayer(DamageAmount);
			/*if(GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1,.5f,FColor::Red,enemyPlayer->GetName());
		
			}*/
			Destroy();
		}
	}
}

void AEnemyProjectile::SetDamageAmount(int NewDamageAmount)
{
	DamageAmount=NewDamageAmount;
}
float AEnemyProjectile::DealDamage(const float Health, const float Damage, const float MaxHealth)
{

	/**if damage would set below 0*/
	if(Health-Damage<=0.0)
	{
		return 0.0;
	}
	/**if "damage" would set above max*/
	else if(Health-Damage>=MaxHealth)
	{
		return MaxHealth;
	}
	/**otherwise deal damage*/
	else
	{
		return Health-Damage;
	}
}
