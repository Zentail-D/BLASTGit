// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponTest/WeaponSystem/ItemParent.h"
#include "EBlueprintEnums.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "WeaponTest/WeaponSystem/ProjectileParent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AI/AIEnemyParent.h"
#include "ModParent.generated.h"

/**
 * 
 */
UCLASS()
class AModParent : public AItemParent
{
	GENERATED_BODY()

public:
	AModParent();
	/**
	 * @param CameraComponent - Reference to the owning players camera component
	 * @param MuzzleLocation Location that the actual VFX will play from
	 * @param OwnersName - String of the owning character actors name
	 * Needs to be override to allow for the implementation of different effects when a fire function is called for separate mods
	 */
	virtual void FireActiveMod(UCameraComponent* CameraComponent, UStaticMeshComponent* MuzzleLocation, FString OwnersName);

	/**Inventory checks this boolean to see if the mod needs to be destroyed*/
	bool bReadyToDestroy = false;
	/**
	* @param CameraComponent - Reference to the owning players camera component
	* @param MuzzleLocation Location that the actual VFX will play from
	* Needs to be override to allow for the implementation of different effects when a release function is called for separate mods
	*/
	virtual void ActiveModRelease(UCameraComponent* CameraComponent, UStaticMeshComponent* MuzzleLocation);

	/** called when projectile overlaps something 
	* @param OverlappedComponent component that overlapped
	* @param OtherActor other actor that was overlapped
	* @param OtherComp other component that overlapped
	* @param OtherBodyIndex index the other object is
	* @param bFromSweep if theres a sweep
	* @param SweepResult hit result from the sweep
	*/
	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	/** calculates the needed direction to fire vfx so that it goes towards the center of the screen
	*  @param CameraComponent - player camera component
	*  @param Muzzle - where the vfx will come out of
	*  @return The direction from muzzle to where the vfx should go as a unit vector
	*/
	FVector GetFireDirection(UCameraComponent* CameraComponent, UStaticMeshComponent* Muzzle) const;
	
	/** String that is what the mod type is
	 * @return String will always be Camel Case
	 */
	virtual FString GetModTypeName();

	/** retrieves the current ammo count of the mod
	 * @return the ammo count
	 */
	inline float GetCurrentAmmoCount() const { return AmmoCount; }
	
	/** The visual aspect of our mod */
	UPROPERTY(EditAnywhere, Category = "Item Settings", meta= (AllowProtectedAccess= "true"))
	UStaticMeshComponent* MeshComponent;

protected:
	virtual void BeginPlay() override;
	/** Camera Shake Used When Firing the Mod */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Settings | Mod Settings | Camera Shake", meta = (AllowProtectedAccess = "true"))
	TSubclassOf<UMatineeCameraShake> ModFireShake;

	/** Method that will call this given camera shake to play on the owning player
	 *@param Shake - The matinee shake that needs to be set in blueprints
	 */
	void PlayerCameraShake(TSubclassOf<UMatineeCameraShake> Shake);
	/**
	 * Boolean to Tell if the mod is ready to be fired
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Settings | Mod Settings", meta = (AllowProtectedAccess = "true"))
	bool bReadyToFire = false;

	/**
	 * Amount of Damage the projectile fired from the weapon will do
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Settings | Mod Settings", meta = (AllowProtectedAccess = "true"))
	int ProjectileDamage;

	/**
	 * The amount of shots that a mod will have before it will disappear
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Settings | Mod Settings", meta = (AllowProtectedAccess = "true"))
	float AmmoCount;
	/**
	* Boolean to tell if the mod doesnt use AmmoCount
	*/
	bool bHasInfiniteAmmo = false;
	/**
	 * Cooldown for firing bullets
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Settings | Mod Settings", meta = (AllowProtectedAccess = "true"))
	float RateOfFire;
	/**
	* Used to reset cooldown for firing bullets
	*/
	float RateOfFireReset;
	/**
	 * The speed of the projectile
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Settings | Mod Settings", meta = (AllowProtectedAccess = "true"))
	int ProjectileSpeed;

	/**
	 * The amount of time a projectile will last before they destroy themselves
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Settings | Mod Settings", meta = (AllowProtectedAccess = "true"))
	float ProjectileLifeTime;
	
	/**
	 * For later implementation for status effect if needed
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Settings | Mod Settings", meta = (AllowProtectedAccess = "true"))
	EWeaponStatusEffects WeaponStatusEffects;
	/**
	* Niagara component for projectiles
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Settings | Mod Settings", meta = (AllowProtectedAccess = "true"))
	UNiagaraComponent* ProjectileVfxNiagaraComponent;
	/**
	* Niagara System for projectiles
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Settings | Mod Settings", meta = (AllowProtectedAccess = "true"))
	UNiagaraSystem* ProjectileVFXNiagaraSystem;
	
	/**
	* Niagara system for muzzle effects
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Settings | Mod Settings", meta = (AllowProtectedAccess = "true"))
	UNiagaraSystem* MuzzleVFXNiagaraSystem;
	
	/**
	* Niagara system for collision
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Settings | Mod Settings", meta = (AllowProtectedAccess = "true"))
	UNiagaraSystem* CollisionVFXNiagaraSystem;

	/**
	 * Projectile that the mod spawns in
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Settings | Mod Settings", meta = (AllowProtectedAccess = "true"))
	TSubclassOf<AProjectileParent> ProjectileClass;

	/**
	* Fire Sound to play
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Settings | Mod Settings", meta = (AllowProtectedAccess = "true"))
	USoundBase* FireSound;

	/**
	* Release Sound to play
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Settings | Mod Settings", meta = (AllowProtectedAccess = "true"))
	USoundBase* ReleaseSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Settings | Mod Settings", meta = (AllowProtectedAccess = "true"))
	float MinimumTraceThreshold = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Settings | Mod Settings", meta = (AllowProtectedAccess = "true"))
	float MaxTraceThreshold = 5000.f;

	/** Name of the character actor that owns this mod and is wanting to fire it right now */
	FString OwnerName = "None";

	/** ----- CUSTOM SCREEN SHAKE VARIABLES ----- **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Settings | Mod Settings | Screen Shake", meta = (AllowProtectedAccess = "true"))
	float OscillationDuration = 0.25f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Settings | Mod Settings | Screen Shake", meta = (AllowProtectedAccess = "true"))
	float OscillationBlendInTime = 0.05f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Settings | Mod Settings | Screen Shake", meta = (AllowProtectedAccess = "true"))
	float OscillationBlendOutTime = 0.05f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Settings | Mod Settings | Screen Shake | Pitch Settings", meta = (AllowProtectedAccess = "true"))
	float MinPitchAmplitude = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Settings | Mod Settings | Screen Shake | Pitch Settings", meta = (AllowProtectedAccess = "true"))
	float MaxPitchAmplitude = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Settings | Mod Settings | Screen Shake | Pitch Settings", meta = (AllowProtectedAccess = "true"))
	float MinPitchFrequency = 25.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Settings | Mod Settings | Screen Shake | Pitch Settings", meta = (AllowProtectedAccess = "true"))
	float MaxPitchFrequency = 35.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Settings | Mod Settings | Screen Shake | Yaw Settings", meta = (AllowProtectedAccess = "true"))
	float MinYawAmplitude = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Settings | Mod Settings | Screen Shake | Yaw Settings", meta = (AllowProtectedAccess = "true"))
	float MaxYawAmplitude = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Settings | Mod Settings | Screen Shake | Yaw Settings", meta = (AllowProtectedAccess = "true"))
	float MinYawFrequency = 25.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Settings | Mod Settings | Screen Shake | Yaw Settings", meta = (AllowProtectedAccess = "true"))
	float MaxYawFrequency = 35.0f;
	
};
