// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponTest/WeaponSystem/ProjectileParent.h"
#include "RailgunProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ARailgunProjectile : public AProjectileParent
{
	GENERATED_BODY()
public:
	ARailgunProjectile();
	
	/** called when projectile overlaps something 
	* @param OverlappedComponent component that overlapped
	* @param OtherActor other actor that was overlapped
	* @param OtherComp other component that overlapped
	* @param OtherBodyIndex index the other object is
	* @param bFromSweep if theres a sweep
	* @param SweepResult hit result from the sweep
	*/
	
	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;

	virtual void Tick(float DeltaSeconds) override;

protected:

	virtual void BeginPlay() override;
	
};
