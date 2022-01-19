// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponSystem/ItemParent.h"
#include "EBlueprintEnums.h"
#include "AIEnemyParent.h"
#include "NiagaraFunctionLibrary.h"
#include "ProjectileParent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ModParent.generated.h"

/**
 * 
 */
UCLASS()
class BLAST_API AModParent : public AItemParent
{
	GENERATED_BODY()

public:
	AModParent();
	/**
	 * @param CameraComponent - Reference to the owning players camera component
	 * @param MuzzleLocation Location that the actual VFX will play from
	 * Needs to be override to allow for the implementation of different effects when a fire function is called for separate mods
	 */
	virtual void FireActiveMod(UCameraComponent* CameraComponent, UStaticMeshComponent* MuzzleLocation);

	/**Inventory checks this boolean to see if the mod needs to be destoryed*/
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

	/** String that is what the mod type is
	 * @return String will always be CammelCase
	 */
	virtual FString GetModTypeName();
	
	/** The visual aspect of our mod */
	UPROPERTY(EditAnywhere, Category = "Item Settings", meta= (AllowProtectedAccess= "true"))
	UStaticMeshComponent* MeshComponent;

protected:
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
	int AmmoCount;
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
	
};
