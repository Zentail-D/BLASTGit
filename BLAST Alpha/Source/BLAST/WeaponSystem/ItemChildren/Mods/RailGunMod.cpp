// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponSystem/ItemChildren/Mods/RailGunMod.h"

#include "DrawDebugHelpers.h"

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
		if(RateOfFire>0.0f)
		{
			RateOfFire -= DeltaTime;
			
		}
		else
		{
			bReadyToFire = true;
			RateOfFire = RateOfFireReset;
		}
	}
	if(AmmoCount<=0)
	{
		bReadyToDestroy=true;
	}
}

void ARailGunMod::FireActiveMod(UCameraComponent* CameraComponent, UStaticMeshComponent* MuzzleLocation)
{
	if(FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, MuzzleLocation->GetComponentLocation());
	}
	bIsCharging=true;
}

void ARailGunMod::ActiveModRelease(UCameraComponent* CameraComponent, UStaticMeshComponent* MuzzleLocation)
{
	bIsCharging=false;
	if(bReadyToFire)
	{
		bReadyToFire = false;
		ProjectileVfxNiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),ProjectileVFXNiagaraSystem,MuzzleLocation->GetComponentLocation(),GetFireDirection(CameraComponent, MuzzleLocation).Rotation());
		ProjectileVfxNiagaraComponent->SetFloatParameter("User.Lifetime", ProjectileLifeTime);
		ProjectileVfxNiagaraComponent->SetVectorParameter("User.Velocity", FVector(ProjectileSpeed, 0.f, 0.f));
		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator = GetInstigator();
		AProjectileParent* ProjectileParent = GetWorld()->SpawnActor<AProjectileParent>(ProjectileClass,CameraComponent->GetComponentLocation(), FRotator(0,0,0), SpawnParams);
		if(ProjectileParent)
		{
			if(ReleaseSound)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), ReleaseSound, MuzzleLocation->GetComponentLocation());
			}
			// set our initial and max speed of our projectile
			ProjectileParent->ProjectileMovementComponent->InitialSpeed = ProjectileSpeed;
			ProjectileParent->ProjectileMovementComponent->MaxSpeed = ProjectileSpeed;
			ProjectileParent->FireInDirection(CameraComponent->GetComponentRotation().Vector());
			ProjectileParent->SetDamageAmount(ProjectileDamage);
			ProjectileParent->SetProjectileLifespan(ProjectileLifeTime);	// set our lifetime
		}
		if (AmmoCount>0)
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

FVector ARailGunMod::GetFireDirection(UCameraComponent* CameraComponent, UStaticMeshComponent* Muzzle) const
{
	// set some variables that will be needed
	FVector MuzzleLocation = Muzzle->GetComponentLocation();
	FVector CameraLookDirection = CameraComponent->GetForwardVector().GetSafeNormal();
	FVector CameraLocation = CameraComponent->GetComponentLocation();
	FVector ReturnDirection;
	float Distance;

	//start of ray
	FVector TraceStart = CameraLocation;
	FVector TraceEnd = CameraLocation + CameraLookDirection * MaxTraceThreshold;
	//ignore player that is holding the mod
	const AActor* ActorToIgnore = GetInstigator();
	//set up trace parameters
	FCollisionQueryParams TraceParams(FName(TEXT("RayTracing")), true, ActorToIgnore);
	TraceParams.bReturnPhysicalMaterial = false;
	TraceParams.bTraceComplex = true;

	FHitResult HitResult;
	
	//ray trace check for first collision 
	bool HadHit = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, TraceParams);
				
	if (HadHit)
	{
		// get new location to move to since we would have been running into a wall
		FVector VectorToHitLocation = HitResult.Location - CameraLocation;
		float HitDistance = (VectorToHitLocation).Size();
		
		if (HitDistance<=MinimumTraceThreshold)		// we are closer than our threshold allows
		{
			Distance = MinimumTraceThreshold;
			// return unit vector from muzzle location to location at min distance in front of camera
			ReturnDirection = ((CameraLocation + (CameraLookDirection * MinimumTraceThreshold)) - MuzzleLocation).GetSafeNormal();
		} else		// hit location is far enough away
		{
			Distance = HitDistance;
			// return unit vector from muzzle location to hit location
			ReturnDirection = (HitResult.Location - MuzzleLocation).GetSafeNormal();	// return unit vector from muzzle location to hit location
		}
	} else
	{
		Distance = MaxTraceThreshold;
		// we have not hit so return default as such
		// return unit vector from muzzle location to location at min distance in front of camera
		ReturnDirection = (TraceEnd - MuzzleLocation).GetSafeNormal();
	}
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::SanitizeFloat(Distance));

	//DrawDebugLine(GetWorld(), CameraLocation, MuzzleLocation + ReturnDirection * Distance, FColor::Red, false, 1.5f, 0, 5);
	//DrawDebugLine(GetWorld(), MuzzleLocation, MuzzleLocation + ReturnDirection * Distance, FColor::Red, false, 1.5f, 0, 5);
	//DrawDebugCircle(GetWorld(), MuzzleLocation + ReturnDirection * Distance, 200, 50, FColor::Red, false, 30.f);

	return ReturnDirection;
	
}
