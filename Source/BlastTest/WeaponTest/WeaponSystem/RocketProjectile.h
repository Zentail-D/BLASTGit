// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "WeaponTest/WeaponSystem/ProjectileParent.h"
#include "RocketProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ARocketProjectile : public AProjectileParent
{
	GENERATED_BODY()
public:
	ARocketProjectile();
	
	/** called when projectile overlaps something 
	* @param OverlappedComponent component that overlapped
	* @param OtherActor other actor that was overlapped
	* @param OtherComp other component that overlapped
	* @param OtherBodyIndex index the other object is
	* @param bFromSweep if theres a sweep
	* @param SweepResult hit result from the sweep
	*/
	
	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;

	
	/**
	*Setter for the Damage that the bullet does
	*@param NewExplosionRadius New radius  the explosion needs to effect
	*/
	void SetExplosionRadius(float NewExplosionRadius);

	

	float GetExplosionRadius() const;
protected:

	virtual void BeginPlay() override;

	
	
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* SphereCollider;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Settings | Mod Settings | Rocket Launcher Unique", meta = (AllowProtectedAccess = "true"))
	float ExplosionRadius=500.0f;
	/**Object types that the Area of Effect Damage Affects*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Settings | Mod Settings | Rocket Launcher Unique", meta = (AllowProtectedAccess = "true"))
	TArray<TEnumAsByte<EObjectTypeQuery> > AffectedObjectTypes;
	/**Actors that don't get hit by the Area of Effect Damage*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Settings | Mod Settings | Rocket Launcher Unique", meta = (AllowProtectedAccess = "true"))
	TArray<AActor*> ActorsToIgnore;
};
