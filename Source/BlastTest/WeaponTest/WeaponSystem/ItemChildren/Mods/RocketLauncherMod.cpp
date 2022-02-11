// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponTest/WeaponSystem/ItemChildren/Mods/RocketLauncherMod.h"
#include "Kismet/GameplayStatics.h"
#include "WeaponTest/WeaponSystem/RocketProjectile.h"
#include "NetworkChar.h"
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
			if(Cast<ANetworkChar>(GetInstigator())->AudioComponent)
			{
				if(Cast<ANetworkChar>(GetInstigator())->AudioComponent->IsPlaying())
				{
					Cast<ANetworkChar>(GetInstigator())->AudioComponent->FadeOut(0.1,0);
				}
				if(OutOfAmmoSound)
				{
					Cast<ANetworkChar>(GetInstigator())->AudioComponent->SetSound(OutOfAmmoSound);
					Cast<ANetworkChar>(GetInstigator())->AudioComponent->FadeIn(0.1f);
				}
			}
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
		ProjectileVfxNiagaraComponent->SetVectorParameter("User.Velocity",FVector(ProjectileSpeed, 0.f, 0.f));
		FVector CollisionVector = GetFireDirection(CameraComponent, MuzzleLocation)*-1;
		CollisionVector*= ProjectileMuzzleOffset;
		FTransform CollisionTransform =FTransform(FRotator(0,0,0),CollisionVector,FVector(0,0,0)); 
		ARocketProjectile* ProjectileParent = GetWorld()->SpawnActorDeferred<ARocketProjectile>(ProjectileClass, MuzzleLocation->GetComponentTransform()+CollisionTransform, GetOwner(), GetInstigator());
		
		if(ProjectileParent)
		{
			
			if(Cast<ANetworkChar>(GetInstigator())->AudioComponent)
			{
				if(FireSound)
				{
					//GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Purple,"Firing");
					Cast<ANetworkChar>(GetInstigator())->AudioComponent->SetWorldLocation(GetInstigator()->GetActorLocation());
					Cast<ANetworkChar>(GetInstigator())->AudioComponent->SetSound(FireSound);
					Cast<ANetworkChar>(GetInstigator())->AudioComponent->FadeIn(0.1f);
				}
				else
				{
					GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Purple,"No FireSound");
				}
			}
			ProjectileParent->OwningPlayer = OwningPlayer;	// let the projectile know what the owning inventory is
			ProjectileParent->SetDamageAmount(ProjectileDamage);
			ProjectileParent->SetImpulsePower(ProjectileImpulse);
			ProjectileParent->SetExplosionRadius(ExplosionRadius);
			ProjectileParent->SetExplosionStrength(ExplosionStrength);
			ProjectileParent->SetOwnersName(OwnersName);
			ProjectileParent->SetInstigator(GetInstigator());
			

			// Finish spawning actor now
			UGameplayStatics::FinishSpawningActor(ProjectileParent, MuzzleLocation->GetComponentTransform()+CollisionTransform);
			// we fire in direction after the actor is officailly spawned
			ProjectileParent->FireInDirection(CameraComponent->GetComponentRotation().Vector());
			
		}
		this->PlayerCameraShake(ModFireShake, 1.0f);
		if (AmmoCount>0 && !bIsDefaultMod)
		{
			AmmoCount--;
		}
	
	}
}

FString ARocketLauncherMod::GetModTypeName()
{
	return FString("Rocket Launcher");
}

