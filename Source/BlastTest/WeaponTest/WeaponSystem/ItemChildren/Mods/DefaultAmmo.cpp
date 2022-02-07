// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponTest/WeaponSystem/ItemChildren/Mods/DefaultAmmo.h"

#include "NetworkChar.h"

ADefaultAmmo::ADefaultAmmo()
{
	//set up defaults(can be overriden in blueprints)
	bReadyToFire = true;
	ProjectileDamage= 15;
	bHasInfiniteAmmo =true;
	RateOfFire = 0.1f;
	RateOfFireReset =RateOfFire;
	ProjectileSpeed = 3000;
	ProjectileLifeTime = 1.0f;
	ProjectileImpulse = 100;

}

void ADefaultAmmo::Tick(float DeltaTime)
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
	
}

void ADefaultAmmo::FireActiveMod(UCameraComponent* CameraComponent, UStaticMeshComponent* MuzzleLocation, FString OwnersName)
{
	// if not initialized then set who our owner is for this instance of the mod
	if (OwnerName == "None")
	{
		OwnerName = OwnersName;
	}
	
	if(bReadyToFire)
	{
		bReadyToFire = false;
		ProjectileVfxNiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),ProjectileVFXNiagaraSystem,MuzzleLocation->GetComponentLocation(),GetFireDirection(CameraComponent, MuzzleLocation).Rotation());
		ProjectileVfxNiagaraComponent->SetFloatParameter("User.Lifetime",ProjectileLifeTime);
		ProjectileVfxNiagaraComponent->SetVectorParameter("User.Velocity",FVector(ProjectileSpeed, 0.f, 0.f));
		FVector CollisionVector = GetFireDirection(CameraComponent, MuzzleLocation)*-1;
		CollisionVector*= ProjectileMuzzleOffset;
		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator=GetInstigator();
		AProjectileParent* ProjectileParent = GetWorld()->SpawnActor<AProjectileParent>(ProjectileClass,MuzzleLocation->GetComponentLocation()+CollisionVector, FRotator(0,0,0), SpawnParams);
		if(ProjectileParent)
		{
			if(FireSound)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, MuzzleLocation->GetComponentLocation());
			}
			ProjectileParent->SetProjectileLifespan(ProjectileLifeTime);
			ProjectileParent->FireInDirection(CameraComponent->GetComponentRotation().Vector());
			ProjectileParent->SetDamageAmount(ProjectileDamage);
			ProjectileParent->SetImpulsePower(ProjectileImpulse);
			ProjectileParent->SetOwnersName(OwnersName);
			ProjectileParent->SetInstigator(GetInstigator());
		}
		// play our screen shake
		PlayerCameraShake(ModFireShake, 1.0f);
		//KnockBackPlayer(ModFireKnockBackForce, CameraComponent);
	}
}

FString ADefaultAmmo::GetModTypeName()
{
	return FString("Default");
}



