// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponTest/WeaponSystem/ItemChildren/Mods/ModParent.h"
#include "ShotgunMod.generated.h"

/**
 * 
 */
UCLASS()
class AShotgunMod : public AModParent
{
	GENERATED_BODY()
	public:
	AShotgunMod();
	/**
    	*Ran Every Frame and Execute any code in this function every frame.
    	*@param DeltaTime the amount of time that has passed since the last frame to use in calculations
    	*/
    virtual void Tick(float DeltaTime) override;
	/**
	 * Plays VFX in the direction the cameras facing
	 * @param CameraComponent direction and location the camera is facing
	 * @param MuzzleLocation location of where the VFX is played from.
	 */
    virtual void FireActiveMod(UCameraComponent* CameraComponent, UStaticMeshComponent* MuzzleLocation) override;

	/** Returns the name of the move for the user interface
	* @return - the name of the mod that will appear in the player's UI
	*/
	virtual FString GetModTypeName() override;
	
    protected:
	/**number of pellets in the shotgun shell*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Settings | Mod Settings | Shotgun Unique", meta = (AllowProtectedAccess = "true"))
	int32 projectilesToSpawn = 10;
	/** Leftmost spread angle for the X axis in degrees( -90=left, 0 is straight)*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Settings | Mod Settings | Shotgun Unique", meta = (AllowProtectedAccess = "true"))
	float MinXSpread;
	/** Rightmost spread angle for the X axis in degrees( 0 is straight, 90=right)*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Settings | Mod Settings | Shotgun Unique", meta = (AllowProtectedAccess = "true"))
	float MaxXSpread;
	/** Furthest behind spread angle for the Y axis in degrees DO NOT TOUCH*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Settings | Mod Settings | Shotgun Unique", meta = (AllowProtectedAccess = "true"))
	float MinYSpread =-0.6;
	/** Furthest forward spread angle for the Y axis in degrees DO NOT TOUCH*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Settings | Mod Settings | Shotgun Unique", meta = (AllowProtectedAccess = "true"))
	float MaxYSpread = 0.6;
	/** Lowest spread angle for the Z axis in degrees( -90= down, 0 is straight)*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Settings | Mod Settings | Shotgun Unique", meta = (AllowProtectedAccess = "true"))
	float MinZSpread;
	/** Highest spread angle for the Z axis in degrees( 0 is straight, 90= up)*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Settings | Mod Settings | Shotgun Unique", meta = (AllowProtectedAccess = "true"))
	float MaxZSpread;
   
};
