// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GraphColor/Private/appconst.h"
#include "WeaponTest/WeaponSystem/ItemChildren/Mods/ModParent.h"
#include "LaserMod.generated.h"

/**
 * 
 */
UCLASS()
class ALaserMod : public AModParent
{
	GENERATED_BODY()

public:
	ALaserMod();

	bool bLaserFiring=false;

	/** How far the laser can extend */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Settings | Mod Settings")
	float LaserRange;

	/** how often the laser can inflict damage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Settings | Mod Settings")
	float LaserDamageCooldown;

	/** the stored reset value for the damage cooldown */
	float LaserDamageCooldownReset = LaserDamageCooldown;

	/** This works with the damage cooldown so we know if we are allowed to deal damage */
	bool CanDealDamage = true;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	/**
	* Plays VFX in the direction the cameras facing
	* @param CameraComponent direction and location the camera is facing
	* @param MuzzleLocation location of where the VFX is played from.
	*/
	virtual void FireActiveMod(UCameraComponent* CameraComponent, UStaticMeshComponent* MuzzleLocation) override;

	/**
	* Plays VFX in the direction the cameras facing
	* @param CameraComponent direction and location the camera is facing
	* @param MuzzleLocation location of where the VFX is played from.
	*/
	virtual void ActiveModRelease(UCameraComponent* CameraComponent, UStaticMeshComponent* MuzzleLocation) override;

	/** Returns the name of the move for the user interface
	* @return - the name of the mod that will appear in the player's UI
	*/
	virtual FString GetModTypeName() override;

protected:
	virtual void BeginPlay() override;
	
	UCameraComponent* PlayerCameraComponent;
	UStaticMeshComponent* PlayerMuzzleComponent;
};
