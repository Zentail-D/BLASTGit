// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponSystem/ItemChildren/Mods/LaserMod.h"

#include "NetworkChar.h"
#include "Kismet/KismetSystemLibrary.h"

ALaserMod::ALaserMod()
{
	bReadyToFire = true;
	ProjectileDamage= 15;
	bHasInfiniteAmmo =false;
	RateOfFire = 0.1f;
	RateOfFireReset =RateOfFire;
	LaserDamageCooldownReset=LaserDamageCooldown;
	ProjectileSpeed = 3000;
	ProjectileLifeTime = 1.0f;
}

void ALaserMod::Tick(float DeltaTime)
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
	if(bLaserFiring)
	{
		LaserDamageCooldown-=DeltaTime;
		if(ProjectileVfxNiagaraComponent)
		{
			if(PlayerCameraComponent && PlayerMuzzleComponent)
			{
				ProjectileVfxNiagaraComponent->SetWorldLocation(PlayerMuzzleComponent->GetComponentLocation());
				TArray<AActor*> IgnoreActors;
				IgnoreActors.Push(GetInstigator());
				FHitResult HitResult;
				/*const bool HadHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(),PlayerMuzzleComponent->GetComponentLocation()
				, PlayerMuzzleComponent->GetComponentLocation()+PlayerCameraComponent->GetComponentRotation().Vector()*LaserRange,ETraceTypeQuery::TraceTypeQuery1,
				false,IgnoreActors,EDrawDebugTrace::ForDuration,HitResult,true,
				FLinearColor::Red,FLinearColor::Green,5);
				if(HadHit)
				{
					ProjectileVfxNiagaraComponent->SetVectorParameter("User.End",HitResult.Location);
					if(LaserDamageCooldown<0)
					{
						 if(Cast<ANetworkChar>(HitResult.Actor))
						 {
						 	Cast<ANetworkChar>(HitResult.Actor)->DealDamageToPlayer(ProjectileDamage);
						 	LaserDamageCooldown=LaserDamageCooldownReset;
						 }
						else if(Cast<AAIEnemyParent>(HitResult.Actor))
						{
							Cast<AAIEnemyParent>(HitResult.Actor)->DealDamageToEnemy(ProjectileDamage);
							LaserDamageCooldown=LaserDamageCooldownReset;
						}
					}
				}
				else
				{
					ProjectileVfxNiagaraComponent->SetVectorParameter("User.End",PlayerMuzzleComponent->GetComponentLocation()+PlayerCameraComponent->GetComponentRotation().Vector()*LaserRange);
				}
				AmmoCount--;*/
			}
		}
		//GEngine->AddOnScreenDebugMessage(-1,0.01,FColor::Red,TEXT("You holding the fire button"));
	}
}

void ALaserMod::FireActiveMod(UCameraComponent* CameraComponent, UStaticMeshComponent* MuzzleLocation)
{
	if(bReadyToFire)
	{
		if(FireSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, MuzzleLocation->GetComponentLocation());
		}
		bReadyToFire = false;
		bLaserFiring = true;
		if(!PlayerCameraComponent)
			PlayerCameraComponent=CameraComponent;
		if(!PlayerMuzzleComponent)
			PlayerMuzzleComponent=MuzzleLocation;

		TArray<AActor*> IgnoreActors;
		IgnoreActors.Push(GetInstigator());
		
		ProjectileVfxNiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),ProjectileVFXNiagaraSystem,MuzzleLocation->GetComponentLocation(),CameraComponent->GetComponentRotation());
		FHitResult HitResult;
		const bool HadHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(),MuzzleLocation->GetComponentLocation()
		, MuzzleLocation->GetComponentLocation()+CameraComponent->GetComponentRotation().Vector()*LaserRange,ETraceTypeQuery::TraceTypeQuery1,
		false,IgnoreActors,EDrawDebugTrace::ForDuration,HitResult,true,
		FLinearColor::Red,FLinearColor::Green,5);
		if(HadHit)
		{
			ProjectileVfxNiagaraComponent->SetVectorParameter("User.End",HitResult.Location);
			if(Cast<ANetworkChar>(HitResult.Actor))
			{
				Cast<ANetworkChar>(HitResult.Actor)->DealDamageToPlayer(ProjectileDamage);
				LaserDamageCooldown=LaserDamageCooldownReset;
			}
			else if(Cast<AAIEnemyParent>(HitResult.Actor))
			{
				Cast<AAIEnemyParent>(HitResult.Actor)->DealDamageToEnemy(ProjectileDamage);
				LaserDamageCooldown=LaserDamageCooldownReset;
			}
		}
		else
		{
			ProjectileVfxNiagaraComponent->SetVectorParameter("User.End",MuzzleLocation->GetComponentLocation()+CameraComponent->GetComponentRotation().Vector()*LaserRange);
		}
		AmmoCount--;
	}
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
