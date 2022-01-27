// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponTest/WeaponSystem/ItemChildren/Mods/RocketLauncherMod.h"
#include "Kismet/GameplayStatics.h"
#include "WeaponTest/WeaponSystem/RocketProjectile.h"
#include "Kismet/KismetSystemLibrary.h"

ARocketLauncherMod::ARocketLauncherMod()
{
	//set up defaults(can be overriden in blueprints)
	bReadyToFire = true;
	ProjectileDamage= 50;
	AmmoCount = 10;
	RateOfFire = 0.6f;
	RateOfFireReset =RateOfFire;
	ProjectileSpeed = 4000;
	ProjectileLifeTime = 1.5f;
}

void ARocketLauncherMod::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(!bReadyToFire)
	{
		if(RateOfFire>0.0f)
		{
			RateOfFire-=DeltaTime;
		}
		else
		{
			bReadyToFire=true;
			RateOfFire=RateOfFireReset;
		}
		if(AmmoCount<=0)
		{
			bReadyToDestroy = true;
		}
	}
}

void ARocketLauncherMod::FireActiveMod(UCameraComponent* CameraComponent, UStaticMeshComponent* MuzzleLocation, FString OwnersName)
{
	// if not initialized then set who our owner is for this instance of the mod
	if (OwnerName == "None")
	{
		OwnerName = OwnersName;
	}
	
	if(bReadyToFire)
	{
		bReadyToFire = false;
		//test
		ProjectileVfxNiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),ProjectileVFXNiagaraSystem,MuzzleLocation->GetComponentLocation(),CameraComponent->GetComponentRotation());
		ProjectileVfxNiagaraComponent->SetFloatParameter("User.Lifetime",ProjectileLifeTime);
		
		ARocketProjectile* ProjectileParent = GetWorld()->SpawnActorDeferred<ARocketProjectile>(ProjectileClass, CameraComponent->GetComponentTransform(), GetOwner(), GetInstigator());
		
		if(ProjectileParent)
		{
			
			if(FireSound)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, MuzzleLocation->GetComponentLocation());
			}
			ProjectileParent->SetDamageAmount(ProjectileDamage);
			ProjectileParent->SetExplosionRadius(ExplosionRadius);
			ProjectileParent->SetOwnersName(OwnersName);
			ProjectileParent->SetInstigator(GetInstigator());

			// Finish spawning actor now
			UGameplayStatics::FinishSpawningActor(ProjectileParent, CameraComponent->GetComponentTransform());
			// we fire in direction after the actor is officailly spawned
			ProjectileParent->FireInDirection(CameraComponent->GetComponentRotation().Vector());
			
		}
		this->PlayerCameraShake(ModFireShake, 1.0f);
		if (AmmoCount>0)
		{
			AmmoCount--;
		}
	
	}
}

FString ARocketLauncherMod::GetModTypeName()
{
	return FString("Rocket Launcher");
}

