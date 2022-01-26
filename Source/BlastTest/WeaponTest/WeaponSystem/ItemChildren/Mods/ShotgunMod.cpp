// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponTest/WeaponSystem/ItemChildren/Mods/ShotgunMod.h"
#include "WeaponTest/WeaponSystem/ShotgunProjectile.h"

AShotgunMod::AShotgunMod()
{
	//set up defaults(can be overriden in blueprints)
	bReadyToFire = true;
	ProjectileDamage= 10;
	AmmoCount = 20;
	RateOfFire = 0.2f;
	RateOfFireReset =RateOfFire;
	ProjectileSpeed = 3000;
	ProjectileLifeTime = 0.5f;
	
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

void AShotgunMod::FireActiveMod(UCameraComponent* CameraComponent, UStaticMeshComponent* MuzzleLocation, FString OwnersName)
{
	// if not initialized then set who our owner is for this instance of the mod
	if (OwnerName == "None")
	{
		OwnerName = OwnersName;
	}
	
	if(bReadyToFire)
	{
		bReadyToFire = false;
		ProjectileVfxNiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),ProjectileVFXNiagaraSystem,MuzzleLocation->GetComponentLocation(),CameraComponent->GetComponentRotation());
		ProjectileVfxNiagaraComponent->SetVectorParameter("User.Angle",CameraComponent->GetComponentRotation().Vector());
		ProjectileVfxNiagaraComponent->SetFloatParameter("User.Lifetime",ProjectileLifeTime);
		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator=GetInstigator();
		//spawn each projectile collision box
		for(int i = 0; i<projectilesToSpawn; i++)
		{
			//calculate fire direction vector for current Projectile
			float x=FMath::RandRange(MinXSpread/6,MaxXSpread/6);
			float y=FMath::RandRange(MinYSpread/6,MaxYSpread/6);
			float z=FMath::RandRange(MinZSpread/6,MaxZSpread/6);
			FVector spread = FVector( x, y,z );

			AShotgunProjectile* ShotgunProjectile = GetWorld()->SpawnActor<AShotgunProjectile>(ProjectileClass,MuzzleLocation->GetComponentLocation(), FRotator(0,0,0), SpawnParams);
			if(ShotgunProjectile)
			{
				if(FireSound)
				{
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, MuzzleLocation->GetComponentLocation());
				}
				ShotgunProjectile->FireInDirection(CameraComponent->GetComponentRotation().Vector()+spread);
				ShotgunProjectile->SetDamageAmount(ProjectileDamage);
				ShotgunProjectile->SetInstigator(GetInstigator());
			}
			
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
