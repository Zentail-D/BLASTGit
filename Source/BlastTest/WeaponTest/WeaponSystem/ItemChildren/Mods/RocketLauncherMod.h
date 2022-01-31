// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "WeaponTest/WeaponSystem/ItemChildren/Mods/ModParent.h"
#include "RocketLauncherMod.generated.h"

/**
 * 
 */
UCLASS()
class ARocketLauncherMod : public AModParent
{
	GENERATED_BODY()
	
	public:
	
	ARocketLauncherMod();
	/**
    	*Ran Every Frame and Execute any code in this function every frame.
    	*@param DeltaTime the amount of time that has passed since the last frame to use in calculations
    	*/
	virtual void Tick(float DeltaTime) override;
	/**
	 * Plays VFX in the direction the cameras facing
	 * @param CameraComponent direction and location the camera is facing
	 * @param MuzzleLocation location of where the VFX is played from.
	 * @param OwnersName string of the owning players name who is using this mod
	 */
	virtual void FireActiveMod(UCameraComponent* CameraComponent, UStaticMeshComponent* MuzzleLocation, FString OwnersName) override;

	/** Returns the name of the move for the user interface
	* @return - the name of the mod that will appear in the player's UI
	*/
	virtual FString GetModTypeName() override;
	
protected:
	
	
	/**Radius from impact point that the damage is done*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Settings | Mod Settings | Rocket Launcher Unique", meta = (AllowProtectedAccess = "true"))
	float ExplosionRadius=100.0f;
	/**knockback strength of the explosion */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Settings | Mod Settings | Rocket Launcher Unique", meta = (AllowProtectedAccess = "true"))
	float ExplosionStrength=2000.0f;
	
	/**Object types that the Area of Effect Damage Affects*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Settings | Mod Settings | Rocket Launcher Unique", meta = (AllowProtectedAccess = "true"))
	TArray<TEnumAsByte<EObjectTypeQuery> > AffectedObjectTypes;
	/**Actors that don't get hit by the Area of Effect Damage*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Settings | Mod Settings | Rocket Launcher Unique", meta = (AllowProtectedAccess = "true"))
	TArray<AActor*> ActorsToIgnore;
};
