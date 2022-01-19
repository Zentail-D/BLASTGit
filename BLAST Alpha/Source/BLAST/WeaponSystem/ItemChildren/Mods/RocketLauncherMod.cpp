// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponSystem/ItemChildren/Mods/RocketLauncherMod.h"

#include "RocketProjectile.h"
#include "Kismet/KismetSystemLibrary.h"

ARocketLauncherMod::ARocketLauncherMod()
{
	//set up defaults(can be overriden in blueprints)
	bReadyToFire = true;
	ProjectileDamage= 50;
	AmmoCount = 10;
	RateOfFire = 0.6f;
	RateOfFireReset =RateOfFire;
	ProjectileSpeed = 6000;
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

void ARocketLauncherMod::FireActiveMod(UCameraComponent* CameraComponent, UStaticMeshComponent* MuzzleLocation)
{
	if(bReadyToFire)
	{
		bReadyToFire = false;
		//test
		ProjectileVfxNiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),ProjectileVFXNiagaraSystem,MuzzleLocation->GetComponentLocation(),CameraComponent->GetComponentRotation());
		ProjectileVfxNiagaraComponent->SetFloatParameter("User.Lifetime",ProjectileLifeTime);
		ProjectileVfxNiagaraComponent->SetFloatParameter("User.Velocity",ProjectileSpeed);
		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator=GetInstigator();
		ARocketProjectile* ProjectileParent = GetWorld()->SpawnActor<ARocketProjectile>(ProjectileClass,CameraComponent->GetComponentLocation(), FRotator(0,0,0), SpawnParams);
		if(ProjectileParent)
		{
			
			if(FireSound)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, MuzzleLocation->GetComponentLocation());
			}
			ProjectileParent->ProjectileMovementComponent->InitialSpeed = ProjectileSpeed;
			ProjectileParent->ProjectileMovementComponent->MaxSpeed = ProjectileSpeed;
			ProjectileParent->FireInDirection(CameraComponent->GetComponentRotation().Vector());
			ProjectileParent->SetDamageAmount(ProjectileDamage);
			ProjectileParent->SetExplosionRadius(ExplosionRadius);
			
		}
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

