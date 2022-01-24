// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponTest/WeaponSystem/ItemChildren/Mods/RailGunMod.h"

#include "DrawDebugHelpers.h"

ARailGunMod::ARailGunMod()
{
	//sniper stats?
	bReadyToFire = false;
}

void ARailGunMod::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!bReadyToFire && bIsCharging)
	{
		GEngine->AddOnScreenDebugMessage(-1,0.5f,FColor::Emerald,FString::SanitizeFloat(RateOfFire));
		if(RateOfFire>0.0f)
		{
			RateOfFire -= DeltaTime;
			
		}
		else
		{
			bReadyToFire = true;
			RateOfFire = RateOfFireReset;
		}
	}
	if(AmmoCount<=0)
	{
		bReadyToDestroy=true;
	}
}

void ARailGunMod::FireActiveMod(UCameraComponent* CameraComponent, UStaticMeshComponent* MuzzleLocation, FString OwnersName)
{
	// if not initialized then set who our owner is for this instance of the mod
	if (OwnerName == "None")
	{
		OwnerName = OwnersName;
	}
	if(FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, MuzzleLocation->GetComponentLocation());
	}
	bIsCharging=true;
}

void ARailGunMod::ActiveModRelease(UCameraComponent* CameraComponent, UStaticMeshComponent* MuzzleLocation)
{
	// reset variables
	bIsCharging=false;
	RateOfFire = RateOfFireReset;
	if(bReadyToFire)
	{
		bReadyToFire = false;
		ProjectileVfxNiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),ProjectileVFXNiagaraSystem,MuzzleLocation->GetComponentLocation(),GetFireDirection(CameraComponent, MuzzleLocation).Rotation());
		ProjectileVfxNiagaraComponent->SetFloatParameter("User.Lifetime", ProjectileLifeTime);
		ProjectileVfxNiagaraComponent->SetVectorParameter("User.Velocity", FVector(ProjectileSpeed, 0.f, 0.f));

		AProjectileParent* ProjectileParent = GetWorld()->SpawnActorDeferred<AProjectileParent>(ProjectileClass, CameraComponent->GetComponentTransform(), GetOwner(), GetInstigator());
		
		if(ProjectileParent)
		{
			
			if(ReleaseSound)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), ReleaseSound, MuzzleLocation->GetComponentLocation());
			}
			// set our initial and max speed of our projectile
			ProjectileParent->ProjectileMovementComponent->InitialSpeed = ProjectileSpeed;
			ProjectileParent->ProjectileMovementComponent->MaxSpeed = ProjectileSpeed;
			ProjectileParent->SetDamageAmount(ProjectileDamage);
			ProjectileParent->SetOwnersName(OwnerName);
			ProjectileParent->SetInstigator(GetInstigator());

			// Finish spawning actor now
			UGameplayStatics::FinishSpawningActor(ProjectileParent, CameraComponent->GetComponentTransform());
			// we fire in direction after the actor is officially spawned
			ProjectileParent->FireInDirection(CameraComponent->GetComponentRotation().Vector());
		}
		
		if (AmmoCount>0 && !bNeverDestroy)
		{
			AmmoCount--;
		}
		
	}
}

FString ARailGunMod::GetModTypeName()
{
	return FString("RailGun");
}

void ARailGunMod::BeginPlay()
{
	Super::BeginPlay();

	RateOfFireReset = RateOfFire;
}