// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponTest/WeaponSystem/ItemChildren/Mods/ShotgunMod.h"

#include "NetworkChar.h"
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

void AShotgunMod::FireActiveMod(UCameraComponent* CameraComponent, UStaticMeshComponent* MuzzleLocation)
{

	
	if(bReadyToFire)
	{
		bReadyToFire = false;
		ProjectileVfxNiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),ProjectileVFXNiagaraSystem,MuzzleLocation->GetComponentLocation(),CameraComponent->GetComponentRotation());
		ProjectileVfxNiagaraComponent->SetVectorParameter("User.Angle",CameraComponent->GetComponentRotation().Vector());
		ProjectileVfxNiagaraComponent->SetFloatParameter("User.Lifetime",ProjectileLifeTime);
		ProjectileVfxNiagaraComponent->SetVectorParameter("User.Velocity",FVector(ProjectileSpeed, 0.f, 0.f));
		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator=GetInstigator();
		FVector CollisionVector = GetFireDirection(CameraComponent, MuzzleLocation)*-1;
		CollisionVector*= ProjectileMuzzleOffset;
		FTransform CollisionTransform =FTransform(FRotator(0,0,0),CollisionVector,FVector(0,0,0)); 

		//spawn each projectile collision box
		for(int i = 0; i<projectilesToSpawn; i++)
		{
			//calculate fire direction vector for current Projectile
			float x=FMath::RandRange(MinXSpread/6,MaxXSpread/6);
			float y=FMath::RandRange(MinYSpread/6,MaxYSpread/6);
			float z=FMath::RandRange(MinZSpread/6,MaxZSpread/6);
			FVector spread = FVector( x, y,z );
			
		

			AShotgunProjectile* ShotgunProjectile = GetWorld()->SpawnActorDeferred<AShotgunProjectile>(ProjectileClass,MuzzleLocation->GetComponentTransform()+CollisionTransform, GetOwner(), GetInstigator());
			if(ShotgunProjectile)
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
				ShotgunProjectile->OwningPlayer = OwningPlayer;	// let the projectile know what the owning inventory is
				ShotgunProjectile->SetDamageAmount(ProjectileDamage);
				ShotgunProjectile->SetImpulsePower(ProjectileImpulse);
				ShotgunProjectile->SetInstigator(GetInstigator());
				// Finish spawning actor now
				UGameplayStatics::FinishSpawningActor(ShotgunProjectile, MuzzleLocation->GetComponentTransform()+CollisionTransform);
				ShotgunProjectile->FireInDirection(CameraComponent->GetComponentRotation().Vector()+spread);
			}
			// play our screen shake
			PlayerCameraShake(ModFireShake, 1.0f);
			//KnockBackPlayer(ModFireKnockBackForce, CameraComponent);
		}
		
		if(AmmoCount>0 && !bIsDefaultMod)
		{
			AmmoCount--;
		}
	}
}

FString AShotgunMod::GetModTypeName()
{
	return FString("Shotgun");
}
