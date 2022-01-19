// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponSystem/ItemChildren/Mods/DefaultAmmo.h"




ADefaultAmmo::ADefaultAmmo()
{
	//set up defaults(can be overriden in blueprints)
	bReadyToFire = true;
	ProjectileDamage= 15;
	bHasInfiniteAmmo =true;
	RateOfFire = 0.1f;
	RateOfFireReset =RateOfFire;
	ProjectileSpeed = 5000;
	ProjectileLifeTime = 1.0f;

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

void ADefaultAmmo::FireActiveMod(UCameraComponent* CameraComponent, UStaticMeshComponent* MuzzleLocation)
{
	if(bReadyToFire)
	{
		bReadyToFire = false;
		ProjectileVfxNiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),ProjectileVFXNiagaraSystem,MuzzleLocation->GetComponentLocation(),CameraComponent->GetComponentRotation());
		ProjectileVfxNiagaraComponent->SetFloatParameter("User.Lifetime",ProjectileLifeTime);
		ProjectileVfxNiagaraComponent->SetFloatParameter("User.Velocity",ProjectileSpeed);
		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator=GetInstigator();
		AProjectileParent* ProjectileParent = GetWorld()->SpawnActor<AProjectileParent>(ProjectileClass,CameraComponent->GetComponentLocation(), FRotator(0,0,0), SpawnParams);
		if(ProjectileParent)
		{
			if(FireSound)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, MuzzleLocation->GetComponentLocation());
			}
			ProjectileParent->ProjectileMovementComponent->InitialSpeed = ProjectileSpeed;
			ProjectileParent->ProjectileMovementComponent->MaxSpeed = ProjectileSpeed;
			ProjectileParent->SetProjectileLifespan(ProjectileLifeTime);
			ProjectileParent->FireInDirection(CameraComponent->GetComponentRotation().Vector());
			ProjectileParent->SetDamageAmount(ProjectileDamage);
		}
	}
}

FString ADefaultAmmo::GetModTypeName()
{
	return FString("Default");
}



