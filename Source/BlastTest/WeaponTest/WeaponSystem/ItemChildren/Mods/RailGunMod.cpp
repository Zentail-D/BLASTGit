// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponTest/WeaponSystem/ItemChildren/Mods/RailGunMod.h"

#include "DrawDebugHelpers.h"
#include "NetworkChar.h"

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
		if(ChargeScale<1.0f)
		{
			ChargeScale += DeltaTime * ChargeScreenShakeRampUpScale;
		}
		if(RateOfFire>0.0f)
		{
			RateOfFire -= DeltaTime;
			
		}
		else
		{
			bReadyToFire = true;
			RateOfFire = RateOfFireReset;
		}
		
		if (Cast<ANetworkChar>(GetInstigator()))	// only run this when we have a valid instigator (aka the player is holding it)
			PlayerCameraShake(ModChargeShake, ChargeScale);
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
		FVector CollisionVector = GetFireDirection(CameraComponent, MuzzleLocation)*-1;
		CollisionVector*= ProjectileMuzzleOffset;
		FTransform CollisionTransform =FTransform(FRotator(0,0,0),CollisionVector,FVector(0,0,0)); 
		AProjectileParent* ProjectileParent = GetWorld()->SpawnActorDeferred<AProjectileParent>(ProjectileClass, MuzzleLocation->GetComponentTransform()+CollisionTransform, GetOwner(), GetInstigator());
		
		if(ProjectileParent)
		{
			
			if(Cast<ANetworkChar>(GetInstigator())->AudioComponent)
			{
				if(ReleaseSound)
				{
					//GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Purple,"Firing");
					Cast<ANetworkChar>(GetInstigator())->AudioComponent->SetWorldLocation(GetInstigator()->GetActorLocation());
					Cast<ANetworkChar>(GetInstigator())->AudioComponent->SetSound(ReleaseSound);
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
			ProjectileParent->SetOwnersName(OwnerName);
			ProjectileParent->SetInstigator(GetInstigator());

			// Finish spawning actor now
			UGameplayStatics::FinishSpawningActor(ProjectileParent, MuzzleLocation->GetComponentTransform()+CollisionTransform);
			// we fire in direction after the actor is officially spawned
			ProjectileParent->FireInDirection(CameraComponent->GetComponentRotation().Vector());
		}
		PlayerCameraShake(ModFireShake, 1.0f);	// make sure to play our camera shake fire
		ChargeScale = 0.0f;		// make sure to reset the charge scale when we fire
		if (AmmoCount>0 && !bIsDefaultMod)
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
