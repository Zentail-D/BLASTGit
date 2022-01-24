// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponTest/WeaponSystem/ProjectileParent.h"
#include "DrawDebugHelpers.h"
#include "AIEnemyParent.h"
#include "FlamethrowerProjectile.generated.h"

/**
 * 
 */
UCLASS()
class AFlamethrowerProjectile : public AProjectileParent
{
	GENERATED_BODY()

	AFlamethrowerProjectile();
	
	virtual void Tick(float DeltaSeconds) override;

	protected:

	/*** Scale values
	 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Scale Properties")
	float ScaleRateX;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Scale Properties")
	float ScaleRateY;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Scale Properties")
	float ScaleRateZ;


	/*** Scale max values
	 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Scale Properties")
	float ScaleRateXMax;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Scale Properties")
	float ScaleRateYMax;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Scale Properties")
	float ScaleRateZMax;

	/*** The current scale values
	 */
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Scale Properties")
	FVector ScaleValue;

	/*** Max reached flags.
	 * BP doesn't need to see these right now.
	 */
	bool XMaxed,YMaxed,ZMaxed;

	/*** The Amount of seconds until the next scale
	 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Scale Properties")
	float ScaleInterval;

	/*** The Max amount for the scaleinterval to be reset at.
	 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Scale Properties")
	float ScaleIntervalMax;

	/***Temp for testing number of hits
	 */
	int NumberOfHits;


	/** called when projectile overlaps something 
	* @param OverlappedComponent component that overlapped
	* @param OtherActor other actor that was overlapped
	* @param OtherComp other component that overlapped
	* @param OtherBodyIndex index the other object is
	* @param bFromSweep if theres a sweep
	* @param SweepResult hit result from the sweep
	*/
	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;

};
