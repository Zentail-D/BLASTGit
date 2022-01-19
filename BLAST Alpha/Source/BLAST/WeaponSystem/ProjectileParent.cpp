// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponSystem/ProjectileParent.h"

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
		RootComponent = CollisionComponent;
	}

	if (!ProjectileMovementComponent)
	{
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
		//ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
	}
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

	GEngine->AddOnScreenDebugMessage(-1,1.0,FColor::Emerald,OtherActor->GetName());
	/**if overlap enemy Player*/
	ANetworkChar* EnemyPlayer = Cast<ANetworkChar>(OtherActor);
	if (EnemyPlayer)
	{
		/**deal damage to enemy player	*/
		if(EnemyPlayer!=GetInstigator())
		{
			EnemyPlayer->DealDamageToPlayer(DamageAmount);
			Destroy();
		}
	}

	AAIEnemyParent* EnemyParent = Cast<AAIEnemyParent>(OtherActor);
	if(EnemyParent)
	{
		GEngine->AddOnScreenDebugMessage(-1,1.0,FColor::Emerald,TEXT("hit ai!"));
		EnemyParent->DealDamageToEnemy(DamageAmount);
		Destroy();
	}

	//if overlap with environment
	if(OtherComp->ComponentTags.Contains("Environment"))
	{
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
	//DrawDebugBox(GetWorld(),GetActorLocation(),CollisionComponent->GetScaledBoxExtent(),FColor::Green,false,0.1,0,3);
}

