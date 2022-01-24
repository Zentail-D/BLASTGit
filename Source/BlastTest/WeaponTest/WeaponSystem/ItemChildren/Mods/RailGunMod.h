// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponTest/WeaponSystem/ItemChildren/Mods/ModParent.h"
#include "RailGunMod.generated.h"

/**
 * 
 */
UCLASS()
class ARailGunMod : public AModParent
{
	GENERATED_BODY()
	ARailGunMod();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	/**
	* @param CameraComponent - The camera component of the player holding the inventory
	* @param MuzzleLocation - Location that the actual VFX will play from
	* Needs to be override to allow for the implementation of different effects when a fire function is called for separate mods
	*/
	virtual void FireActiveMod(UCameraComponent* CameraComponent, UStaticMeshComponent* MuzzleLocation, FString OwnersName) override;
	/**
	* @param CameraComponent - The camera component of the player holding the inventory
	* @param MuzzleLocation - Location that the actual VFX will play from
	* Needs to be override to allow for the implementation of different effects when a release function is called for separate mods
	*/
	virtual void ActiveModRelease(UCameraComponent* CameraComponent, UStaticMeshComponent* MuzzleLocation) override;

	/** Returns the name of the move for the user interface
	 * @return - the name of the mod that will appear in the player's UI
	 */
	virtual FString GetModTypeName() override;
	
	/** sets the reset to the player rate of fire */
	virtual void BeginPlay() override;
	
protected:
	/**
	 *  Boolean used to make sure the mod only fires if fully charged 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Settings | Mod Settings", meta = (AllowProtectedAccess = "true"))
	bool bIsCharging = false;
};
