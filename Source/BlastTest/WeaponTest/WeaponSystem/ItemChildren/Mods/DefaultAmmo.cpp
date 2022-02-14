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

void ADefaultAmmo::FireActiveMod(UCameraComponent* CameraComponent, UStaticMeshComponent* MuzzleLocation)
{
	
	
	if(bReadyToFire)
	{
		bReadyToFire = false;
		FVector CollisionVector = GetFireDirection(CameraComponent, MuzzleLocation)*-1;
		CollisionVector*= ProjectileMuzzleOffset;
		FTransform CollisionTransform =FTransform(FRotator(0,0,0),CollisionVector,FVector(0,0,0)); 
		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator=GetInstigator();
		ServerFireProjectile(MuzzleLocation->GetComponentLocation(),MuzzleLocation->GetComponentTransform()+CollisionTransform,GetFireDirection(CameraComponent, MuzzleLocation));
		//KnockBackPlayer(ModFireKnockBackForce, CameraComponent);
	}
}



void ADefaultAmmo::ServerFireProjectile_Implementation(FVector Location,FTransform CollisionTransform, FVector Direction)
{
	//Super::ServerFireProjectile_Implementation(Location,CollisionTransform, Direction);
	ProjectileVfxNiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),ProjectileVFXNiagaraSystem,Location,Direction.Rotation());
	ProjectileVfxNiagaraComponent->SetFloatParameter("User.Lifetime",ProjectileLifeTime);
	ProjectileVfxNiagaraComponent->SetVectorParameter("User.Velocity",FVector(ProjectileSpeed, 0.f, 0.f));
		
	AProjectileParent* ProjectileParent = GetWorld()->SpawnActorDeferred<AProjectileParent>(ProjectileClass,CollisionTransform, OwningPlayer, GetInstigator());
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
		ProjectileParent->SetProjectileLifespan(ProjectileLifeTime);
		ProjectileParent->SetDamageAmount(ProjectileDamage);
		ProjectileParent->SetImpulsePower(ProjectileImpulse);
		ProjectileParent->OwningPlayer = OwningPlayer;	// let the projectile know what the owning inventory is

		// Finish spawning actor now
		UGameplayStatics::FinishSpawningActor(ProjectileParent, CollisionTransform);
		ProjectileParent->FireInDirection(Direction);
	}
	// play our screen shake
	PlayerCameraShake(ModFireShake, 1.0f);
}

bool ADefaultAmmo::ServerFireProjectile_Validate(FVector Location, FTransform CollisionTransform, FVector Direction)
{
	return true; //Super::ServerFireProjectile_Validate(Location, CollisionTransform,Direction);
}

FString ADefaultAmmo::GetModTypeName()
{
	return FString("Default");
}



