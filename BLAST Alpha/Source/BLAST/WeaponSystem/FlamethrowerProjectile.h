// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponSystem/ProjectileParent.h"
#include "DrawDebugHelpers.h"
#include "FlamethrowerProjectile.generated.h"

/**
 * 
 */
UCLASS()
class BLAST_API AFlamethrowerProjectile : public AProjectileParent
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
	
};
