// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponTest/WeaponSystem/ItemChildren/Mods/LaserMod.h"

#include "BlastTestCharacter.h"
#include "DrawDebugHelpers.h"
#include "NetworkChar.h"
#include "Kismet/KismetSystemLibrary.h"

ALaserMod::ALaserMod()
{
	//
}

void ALaserMod::BeginPlay()
{
	Super::BeginPlay();
	LaserDamageCooldownReset = LaserDamageCooldown;
}

void ALaserMod::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!CanDealDamage)		// if we've dealt damage and now have to wait for damage cooldown
	{
		LaserDamageCooldown -= DeltaTime;
		if (LaserDamageCooldown <= 0.f)
		{
			LaserDamageCooldown = LaserDamageCooldownReset;
			CanDealDamage = true;
		}
	}
	if(bLaserFiring)
	{
		if (!bIsDefaultMod)
			AmmoCount-=DeltaTime;
		this->PlayerCameraShake(ModFireShake, 1.0f);
		if(ProjectileVfxNiagaraComponent)	// if our component is valid
		{
			if(PlayerCameraComponent && PlayerMuzzleComponent)	// if we successfully retrieved the components from fire
			{
				// set the base of our laser to muzzle since player is probably moving
				ProjectileVfxNiagaraComponent->SetWorldLocation(PlayerMuzzleComponent->GetComponentLocation());
				// trace params required for line trace, ignore actor prevents from hitting itself
				FCollisionQueryParams TraceParams(FName(TEXT("Enemy")), true, GetInstigator());
				// initialize hit info
				FHitResult HitResult;
				// do trace
				
				const bool HadHit = GetWorld()->LineTraceSingleByChannel(
					HitResult,
					PlayerMuzzleComponent->GetComponentLocation(),
					PlayerMuzzleComponent->GetComponentLocation()+GetFireDirection(PlayerCameraComponent, PlayerMuzzleComponent)*LaserRange,
					ECC_Visibility,
					TraceParams);

				// debug line for showing the laser line
				//DrawDebugLine(GetWorld(), PlayerMuzzleComponent->GetComponentLocation(), PlayerMuzzleComponent->GetComponentLocation()+PlayerCameraComponent->GetComponentRotation().Vector()*LaserRange,
				//	FColor::Red, false, 2.0f);
				
				if(HadHit)
				{
					ProjectileVfxNiagaraComponent->SetVectorParameter("User.End",HitResult.ImpactPoint);
					GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Red, FString("Hit"));
					
					GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Red, HitResult.ImpactPoint.ToString());
					if (HitResult.Actor->ActorHasTag("Enemy") && CanDealDamage)
					{
						// notify the player that we hit an enemy
						if (GetInstigator())
							Cast<ANetworkChar>(GetInstigator())->OnNotifyProjectileHitEnemy();
						
						AAIEnemyParent* Enemy = Cast<AAIEnemyParent>(HitResult.Actor);
						if (Enemy)
						{
							Enemy->DealDamageToEnemy(ProjectileDamage);
							CanDealDamage = false;
						}
					}	
				}
				else
				{
					//GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Red, FString("No Hit"));
					//DrawDebugCircle(GetWorld(), PlayerMuzzleComponent->GetComponentLocation()+GetFireDirection(PlayerCameraComponent, PlayerMuzzleComponent)*LaserRange, 200, 50, FColor::Blue, true, -1, 0, 10);
					ProjectileVfxNiagaraComponent->SetVectorParameter("User.End",HitResult.TraceEnd/*PlayerMuzzleComponent->GetComponentLocation()+GetFireDirection(PlayerCameraComponent, PlayerMuzzleComponent)*LaserRange*/);
					GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Red, HitResult.TraceEnd.ToString());
				}
			}	
				
			
		}
	}
	if (AmmoCount <= 0.0)
	{
		bReadyToDestroy = true;
		ProjectileVfxNiagaraComponent->DestroyInstance();
	}
}

void ALaserMod::FireActiveMod(UCameraComponent* CameraComponent, UStaticMeshComponent* MuzzleLocation, FString OwnersName)
{
	// if not initialized then set who our owner is for this instance of the mod
	if (OwnerName == "None")
	{
		OwnerName = OwnersName;
	}
	
	if(FireSound)	// play sound if possible
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, MuzzleLocation->GetComponentLocation());
	}
	bLaserFiring = true;	// Tick can know that
	// GRAB OUT CAMERA AND MUZZLE SO WE CAN REFERENCE THEM IN THE TICK
	if(!PlayerCameraComponent)
		PlayerCameraComponent=CameraComponent;
	if(!PlayerMuzzleComponent)
		PlayerMuzzleComponent=MuzzleLocation;
	
	// create our vfx component and point to to the correct location
	ProjectileVfxNiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(ProjectileVFXNiagaraSystem,// pass it our system

	
		MuzzleLocation,		// pass it the component to attach to
		NAME_None,//pass it the attach point name
		MuzzleLocation->GetComponentLocation(),		// start point
		GetFireDirection(CameraComponent, MuzzleLocation).Rotation(),// Rotation
		EAttachLocation::SnapToTargetIncludingScale,//how strong it will be centered while moving
		false);		//we destroy someWhere else, so leave as false
}

void ALaserMod::ActiveModRelease(UCameraComponent* CameraComponent, UStaticMeshComponent* MuzzleLocation)
{
	bLaserFiring=false;
	if(ProjectileVfxNiagaraComponent)
	{
		ProjectileVfxNiagaraComponent->DestroyInstance();
	}
	if(ReleaseSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ReleaseSound, MuzzleLocation->GetComponentLocation());
	}
}

FString ALaserMod::GetModTypeName()
{
	return FString("Laser Beam");
}


