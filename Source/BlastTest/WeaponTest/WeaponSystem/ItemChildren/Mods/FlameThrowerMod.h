// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponTest/WeaponSystem/ItemChildren/Mods/ModParent.h"
#include "FlameThrowerMod.generated.h"

/**
 * 
 */
UCLASS()
class AFlameThrowerMod : public AModParent
{
	GENERATED_BODY()

	/*** Constructor
	 */
	AFlameThrowerMod();

	/**
	 * Plays VFX in the direction the cameras facing
	 * @param CameraComponent direction and location the camera is facing
	 * @param MuzzleLocation location of where the VFX is played from.
	 * @param OwnersName - String of the owning character actors name
	 */
	virtual void FireActiveMod(UCameraComponent* CameraComponent, UStaticMeshComponent* MuzzleLocation) override;
	
	/**
	* Plays VFX in the direction the cameras facing
	* @param CameraComponent direction and location the camera is facing
	* @param MuzzleLocation location of where the VFX is played from.
	*/
	virtual void ActiveModRelease(UCameraComponent* CameraComponent, UStaticMeshComponent* MuzzleLocation) override;
	
	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	/** Returns the name of the move for the user interface
	* @return - the name of the mod that will appear in the player's UI
	*/
	virtual FString GetModTypeName() override;
	
protected:
	/*** The saved reference of the CameraComponent and MuzzleLocation of the owning player.
	 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Player Components")
	UCameraComponent* SavedCameraComponent;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Player Components")
	UStaticMeshComponent* SavedMuzzleLocation;

	/***To keep the mod from spawning projectiles when button not held down.
	 *	Will set true on button down
	 *	Will set false on button up.
	 */
	bool bHoldingFireDown = false;

	/*** A timer of sorts to give intervals to the flame projectile spawn.
	 * NOTE: The base class rate of fire value isn't working for some reason.
	 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Firing Properties")
	float RateOfFlames;

	/***The reset value of our Flame's rate of fire.
	 */
	float RateOfFlamesReset;
};
