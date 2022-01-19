// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponSystem/ItemChildren/Mods/ModParent.h"
#include "FlameThrowerMod.generated.h"

/**
 * 
 */
UCLASS()
class BLAST_API AFlameThrowerMod : public AModParent
{
	GENERATED_BODY()

	/*** Constructor
	 */
	AFlameThrowerMod();

	/*
	virtual void FireActiveMod(FVector* CameraDirection, FVector* CameraLocation, FVector* MuzzleLocation) override;

	virtual void ActiveModRelease(FVector* CameraDirection, FVector* CameraLocation, FVector* MuzzleLocation) override;
*/
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
};






