        
// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponTest/WeaponSystem/ItemChildren/Mods/FlameThrowerMod.h"

#include "FlamethrowerProjectile.h"

AFlameThrowerMod::AFlameThrowerMod()
{
	/*** Defaults if needed
	 */
	this->ProjectileSpeed = 1000;
	this->bReadyToFire = false;

	this->RateOfFlames = 0.1f;
	this->RateOfFlamesReset = this->RateOfFlames;

	FVector PrintValue = FVector(this->RateOfFlames);
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, PrintValue.ToString() );
		
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
	if(FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, MuzzleLocation->GetComponentLocation());
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
	//if(ReleaseSound)
	//{
	//	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ReleaseSound, MuzzleLocation->GetComponentLocation());
	//}
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
		
		if(GetWorld())
		{
			if(this->SavedCameraComponent)
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = GetOwner();
				SpawnParams.Instigator=GetInstigator();
				AFlamethrowerProjectile* OurFlameProjectile = GetWorld()->SpawnActor<AFlamethrowerProjectile>(ProjectileClass,SavedMuzzleLocation->GetComponentLocation(), FRotator(0,0,0), SpawnParams);
				if(OurFlameProjectile)
				{
					OurFlameProjectile->SetDamageAmount(ProjectileDamage);
					OurFlameProjectile->FireInDirection(SavedCameraComponent->GetComponentRotation().Vector());
					OurFlameProjectile->SetLifeSpan(ProjectileLifeTime);
					OurFlameProjectile->SetInstigator(GetInstigator());

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
		this->bReadyToDestroy = true;
		ProjectileVfxNiagaraComponent->Deactivate();
	}
	
}

FString AFlameThrowerMod::GetModTypeName()
{
	return FString("Flamethrower");
}

    