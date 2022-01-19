// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponSystem/ItemChildren/Mods/ShotgunMod.h"
#include "ShotgunProjectile.h"
AShotgunMod::AShotgunMod()
{
	//set up defaults(can be overriden in blueprints)
	bReadyToFire = true;
	ProjectileDamage= 10;
	AmmoCount = 20;
	RateOfFire = 0.2f;
	RateOfFireReset =RateOfFire;
	ProjectileSpeed = 5000;
	ProjectileLifeTime = 0.5f;
	Spread = FVector(4,32,32);
	GrowSpeed = 1.0f;
}

void AShotgunMod::Tick(float DeltaTime)
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
	}
	if(AmmoCount<=0)
	{
		bReadyToDestroy = true;
	}
	
}

void AShotgunMod::FireActiveMod(UCameraComponent* CameraComponent, UStaticMeshComponent* MuzzleLocation)
{
	if(bReadyToFire)
	{
		bReadyToFire = false;
		ProjectileVfxNiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),ProjectileVFXNiagaraSystem,MuzzleLocation->GetComponentLocation(),CameraComponent->GetComponentRotation());
		ProjectileVfxNiagaraComponent->SetVectorParameter("User.Angle",CameraComponent->GetComponentRotation().Vector());
		ProjectileVfxNiagaraComponent->SetFloatParameter("User.Lifetime",0.5f);
		ProjectileVfxNiagaraComponent->SetFloatParameter("User.Velocity",ProjectileSpeed);
		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator=GetInstigator();
		AShotgunProjectile* ShotgunProjectile = GetWorld()->SpawnActor<AShotgunProjectile>(ProjectileClass,CameraComponent->GetComponentLocation(), FRotator(0,0,0), SpawnParams);
		if(ShotgunProjectile)
		{
			
			if(FireSound)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, MuzzleLocation->GetComponentLocation());
			}
			ShotgunProjectile->ProjectileMovementComponent->InitialSpeed = ProjectileSpeed;
			ShotgunProjectile->ProjectileMovementComponent->MaxSpeed = ProjectileSpeed;
			ShotgunProjectile->FireInDirection(CameraComponent->GetComponentRotation().Vector());
			ShotgunProjectile->SetDamageAmount(ProjectileDamage);
			ShotgunProjectile->SetSpread(Spread);
			ShotgunProjectile->SetGrowSpeed(GrowSpeed);
		}
		if(AmmoCount>0)
		{
			AmmoCount--;
		}
	}
}

FString AShotgunMod::GetModTypeName()
{
	return FString("Shotgun");
}
