        
// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponTest/WeaponSystem/ItemChildren/Mods/FlameThrowerMod.h"
#include "NetworkChar.h"
#include "FlamethrowerProjectile.h"

AFlameThrowerMod::AFlameThrowerMod()
{
	/*** Defaults if needed
	 */
	this->ProjectileSpeed = 1000;
	this->bReadyToFire = false;

	this->RateOfFlames = 0.1f;
	this->RateOfFlamesReset = this->RateOfFlames;
	
}

void AFlameThrowerMod::FireActiveMod(UCameraComponent* CameraComponent, UStaticMeshComponent* MuzzleLocation,
	FString OwnersName)
{
	//Super::FireActiveMod(CameraComponent, MuzzleLocation, OwnersName);
	/*** Set VFX to run
	 */
	ProjectileVfxNiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),ProjectileVFXNiagaraSystem,MuzzleLocation->GetComponentLocation(), CameraComponent->GetComponentRotation());
	
	/*** Save the initial values that were passed into this function
	 */
	this->SavedCameraComponent = CameraComponent;
	this->SavedMuzzleLocation = MuzzleLocation;

	/*** Turn on the spawning of the projectils and tell the tick to begin counting.
	 */
	bReadyToFire = true;
	bHoldingFireDown = true;

	/***Sound effect on fire
	 */
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
}

void AFlameThrowerMod::ActiveModRelease(UCameraComponent* CameraComponent, UStaticMeshComponent* MuzzleLocation)
{
	//Super::ActiveModRelease(CameraDirection, CameraLocation, MuzzleLocation);

	bReadyToFire = false;

	/** Destroy the VFX being spawned
	 * May need to be done differently than just "DestroyInstance()"
	 * */
	if(ProjectileVfxNiagaraComponent)
	{
		ProjectileVfxNiagaraComponent->Deactivate();
	}

	/*** Halt projectile spawning
	 */
	bHoldingFireDown = false;

	/*** Sound effect on release
	 */
	if(Cast<ANetworkChar>(GetInstigator())->AudioComponent)
	{
		if(FireSound)
		{
			if(Cast<ANetworkChar>(GetInstigator())->AudioComponent->IsPlaying())
			{
				Cast<ANetworkChar>(GetInstigator())->AudioComponent->FadeOut(0.5f,0);
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Purple,"No FireSound");
		}
	}
}


void AFlameThrowerMod::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	/*** Keep particle effects on the muzzle location and have it rotate correctly
	 */
	if(this->SavedMuzzleLocation && ProjectileVfxNiagaraComponent)
		ProjectileVfxNiagaraComponent->SetWorldLocation(SavedMuzzleLocation->GetComponentLocation());
	if(this->SavedCameraComponent && ProjectileVfxNiagaraComponent)
		ProjectileVfxNiagaraComponent->SetWorldRotation(SavedCameraComponent->GetComponentRotation());

	/*** When ready to fire:
	 *			Spawn Projectile
	 *			Decrease Ammocount
	 *			Reset values
	 *	 When not ready to Fire:
	 *			Countdown
	 *			Set ready when countdown completed
	 */
	if(bReadyToFire && bHoldingFireDown)
	{
		this->PlayerCameraShake(ModFireShake, 1.0f);
		if(GetWorld())
		{
			if(this->SavedCameraComponent)
			{
				FActorSpawnParameters SpawnParams;
				//SpawnParams.Owner = GetOwner();
				//SpawnParams.Instigator=GetInstigator();
				//AFlamethrowerProjectile* OurFlameProjectile = GetWorld()->SpawnActor<AFlamethrowerProjectile>(ProjectileClass,SavedMuzzleLocation->GetComponentLocation(), FRotator(0,0,0), SpawnParams);

				FVector CollisionVector = GetFireDirection(SavedCameraComponent, SavedMuzzleLocation)*-1;
				CollisionVector*= ProjectileMuzzleOffset;
				FTransform CollisionTransform =FTransform(FRotator(0,0,0),CollisionVector,FVector(0,0,0)); 
				AFlamethrowerProjectile* ProjectileParent = GetWorld()->SpawnActorDeferred<AFlamethrowerProjectile>(ProjectileClass,SavedMuzzleLocation->GetComponentTransform()+CollisionTransform, OwningPlayer, GetInstigator());
				if(ProjectileParent)
				{
					ProjectileParent->SetDamageAmount(ProjectileDamage);
					ProjectileParent->FireInDirection(SavedCameraComponent->GetComponentRotation().Vector());
					ProjectileParent->SetLifeSpan(ProjectileLifeTime);
					ProjectileParent->SetInstigator(GetInstigator());
					ProjectileParent->OwningPlayer = OwningPlayer;	// let the projectile know what the owning inventory is

					// Finish spawning actor now
					UGameplayStatics::FinishSpawningActor(ProjectileParent, SavedMuzzleLocation->GetComponentTransform()+CollisionTransform);
					ProjectileParent->FireInDirection(SavedCameraComponent->GetComponentRotation().Vector());
					
					if (!bIsDefaultMod)
						this->AmmoCount -= 1;
					this->RateOfFlames = this->RateOfFlamesReset;
				}
			}
		}
		
		bReadyToFire = false;
	}
	else if(!bReadyToFire && bHoldingFireDown)
	{
		if(RateOfFlames > 0.0)
		{
			RateOfFlames -= DeltaSeconds;
		}
		else
		{
			bReadyToFire = true;
		}
	}


	/*** When the ammo has run out
	 */
	if(this->AmmoCount <= 0)
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
		this->bReadyToDestroy = true;
		ProjectileVfxNiagaraComponent->Deactivate();
	}
	
}

FString AFlameThrowerMod::GetModTypeName()
{
	return FString("Flamethrower");
}

    