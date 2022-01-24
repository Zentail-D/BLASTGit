// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponTest/WeaponSystem/FlamethrowerProjectile.h"

#include "BlastTestCharacter.h"

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
	//Super::OnOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

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
			Cast<ABlastTestCharacter>(GetInstigator())->OnProjectileHitEnemy();
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
	
}