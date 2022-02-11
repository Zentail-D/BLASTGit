// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponTest/WeaponSystem/FlamethrowerProjectile.h"

#include "BlastTestCharacter.h"
#include "NetworkChar.h"

AFlamethrowerProjectile::AFlamethrowerProjectile()
{
	/*** Initial Values set that aren't to be set in BP
	 */
	this->ScaleValue = FVector(1,1,1);
	XMaxed = YMaxed = ZMaxed = false;

	CollisionComponent->OnComponentBeginOverlap.Clear();
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AFlamethrowerProjectile::OnOverlap);

	this->NumberOfHits = 0;
}

void AFlamethrowerProjectile::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	
	if(this->ScaleInterval <= 0.0)
	{
		/*** Scale the Values, X Y Z, of ScaleValue until they reach their respective max value
		*	When maxed, A bool will be set true
		*	-Scale Values must be set in BP-
		*/
		
		/*** X Scaling
		 */
		if(this->ScaleValue.X < this->ScaleRateXMax)
			this->ScaleValue.X += this->ScaleRateX;
		else
			XMaxed = true;
		
	
		/*** Y Scaling
		*/
		if(this->ScaleValue.Y < this->ScaleRateYMax)
			this->ScaleValue.Y += this->ScaleRateY;
		else
			YMaxed = true;
	
		/*** Z Scaling
		*/
		if(this->ScaleValue.Z < this->ScaleRateZMax)
			this->ScaleValue.Z += this->ScaleRateZ;
		else
			ZMaxed = true;
	

		/*** Scale the hitbox until all the max values are reached
		*/
		if(!XMaxed && !YMaxed && !ZMaxed)
		{
			this->CollisionComponent->SetWorldScale3D(ScaleValue);
		}

		this->ScaleInterval = this->ScaleIntervalMax;

		//FVector SlowDownForce = this->ProjectileMovementComponent->Velocity*-1;
		//SlowDownForce = SlowDownForce * 0.01;
		//this->ProjectileMovementComponent->SetVelocityInLocalSpace(this->ProjectileMovementComponent->Velocity + SlowDownForce);
	}
	else
	{
		this->ScaleInterval -= DeltaSeconds;
	}

	/*** Debug Box
	 */
	//DrawDebugBox(GetWorld(),GetActorLocation(),CollisionComponent->GetScaledBoxExtent(),FColor::Green,false,0.04,0,3);
}

void AFlamethrowerProjectile::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
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
	//if overlap with environment
	if(OtherActor->Tags.Contains("Environment"))
	{
		if(GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString("Hit Non-Enemy"));
		Destroy();
	}

	//if overlap with enemy
	if(OtherActor->Tags.Contains("Enemy"))
	{
		// notify the player that we hit an enemy
		if (GetInstigator())
			Cast<ANetworkChar>(GetInstigator())->OnNotifyProjectileHitEnemy();
		//if(GEngine)
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString("Hit") + FString::FromInt(this->NumberOfHits));
		AAIEnemyParent* Enemy = Cast<AAIEnemyParent>(OtherActor);
		if (Enemy)
		{
			Enemy->DealDamageToEnemy(DamageAmount);
			Enemy->SetEnemyStatusEffect(EWeaponStatusEffects::Ve_Fire);
			//this->NumberOfHits++;
		}	
	}
	if (OtherActor->Tags.Contains("Player"))
	{
		// This is an enemy player
		ANetworkChar* Player = Cast<ANetworkChar>(OtherActor);
		Player->DealDamageToPlayer(DamageAmount);
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, FString("Hit Player"));
	}
	Destroy();
	
}
