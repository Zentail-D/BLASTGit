// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemParent.h"
#include "WeaponTest/WeaponSystem/ItemChildren/Mods/ModParent.h"
#include "WeaponTest/WeaponSystem/ItemChildren/GeneratorPoints.h"
#include "EBlueprintEnums.h"
#include "InventoryComponent.generated.h"

struct ModInfo
{
	/** General all purpose mod infos */
	FString Name;
	float Damage;
	float AmmoCount, CurrentAmmoCount;
	bool HasInfiniteAmmo;
	float RateOfFire;
	EWeaponStatusEffects StatusEffect;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	// Sets default values for this component's properties
	UInventoryComponent();

	/** This is the method to call when there is an item to be picked up into the inventory
	 * @param ItemToPickup - This is the actual item were picking up to be handled by the method
	 */
	UFUNCTION(BlueprintCallable)
	void PickupItem(AItemParent* ItemToPickup);

	/** calls the active mods fire method
	 * @param CameraComponent - reference to the players camera component to get info out of for mods
	 * @param MuzzleLocation - the muzzle location of the players weapon
	 */
	UFUNCTION(BlueprintCallable)
	void FireActiveMod(UCameraComponent* CameraComponent, UStaticMeshComponent* MuzzleLocation);

	/** calls the active mods released method (important for things like the rail gun
	 * @param CameraComponent - reference to the players camera component to get info out of for mods
	 * @param MuzzleLocation - the muzzle location of the players weapon
	 */
	UFUNCTION(BlueprintCallable)
	void ActiveModReleased(UCameraComponent* CameraComponent, UStaticMeshComponent* MuzzleLocation);

	/** Called when the player wants to switch between mods
	 * @return - The new active mod slot (NOTE: this can still be the old active mod slot aka the player had nothing to swap to)
	*/
	UFUNCTION(BlueprintCallable)
	void SwapWeapons();

	/** Will primarily be called when the player dies.
	 *  All this does is makes sure both mod slots are empty and sets the active mod slot to DEFAULT
	 */
	UFUNCTION(BlueprintCallable)
	void ResetMods();
	
	/** Called when a GeneratorPoint is picked up and we want to add to our generator points */
	inline void AddGeneratorPoint() { CurrentNumOfGeneratorPoints++; }

	/** Decrements player health
	 * @param DamageAmount - How much to decrement by
	 */
	inline void PlayerTakeDamage(const float DamageAmount) { CurrentPlayerHealth -= DamageAmount; }

	/** For when we want to know what mod we currently have active
	* @return FString of name of mod NOTE: THE NAMES WILL BE CammalCase
	*/
	UFUNCTION(BlueprintCallable)
	FString GetActiveModName() const;
	
	virtual void BeginPlay() override;

	/** ----------------- GETTERS ---------------- */
	/** returns us the current number of Generator Points in the inventory *
	 * @return - The current number of generator points
	 */
	inline int GetCurrentNumOfGeneratorPoints() const { return CurrentNumOfGeneratorPoints; }

	/** What is the players current health
	 * @return - The players current health
	 */
	inline float GetCurrentPlayerHealth() const { return CurrentPlayerHealth; }

	/** Gives the max player health */
	inline float GetMaxPlayerHealth() const { return MaxPlayerHealth; }

	/** ---------------- SETTERS ----------------- */
	/** sets what the players max health can be
	 * @param NewMax - What the new max is allowed to be
	 */
	inline void SetMaxPlayerHealth(const float NewMax) { MaxPlayerHealth = NewMax; }

	/** Sets the current player health to the amount given
	 * @param NewHealth - the amount the new health will be set to
	 */
	inline void SetCurrentPlayerHealth(const float NewHealth) { CurrentPlayerHealth = NewHealth; }

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
protected:
	/** ----------------- PROTECTED SETTERS ---------------- */
	/** sets the ActiveModSlot in the inventory to the given NewActiveSlot
	* @param NewActiveSlot - the new slot to set to
	*/
	inline void SetActiveModSlot(EModSlots NewActiveSlot) { ActiveModSlot = NewActiveSlot; }
	
	/** This function is called primarily when a mod is picked up or weapon fired. It simply looks at the active mod
	 * slot and checks to see if we need to update what the current mod slot is
	 */
	void UpdateActiveMod();

	/** If there is an empty mod slot then store the new mod in that
	 * @param NewMod - The new mod that we want to try and add to our inventory
	 */
	void PickupMod(AModParent* NewMod);

	/** this is essentially our win condition, how many generator points the player has */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category= "Inventory")
	int CurrentNumOfGeneratorPoints = 0;

	/**
	 * The class of Default mod so that we can spawn in a version from the blueprint inside the constructor
	 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category= "Inventory")
	TSubclassOf<AModParent> DefaultModClass;
	
	/** The mod slot that never changes and holds our default mod type to shoot when needed */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category= "Inventory")
	AModParent* DefaultMod;

	/** The mod that is in our slot one */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category= "Inventory")
	AModParent* ModSlot1;

	/** The mod that is in our slot two */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category= "Inventory")
	AModParent* ModSlot2;

	/** The enum that represents what we currently have active */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category= "Inventory")
	EModSlots ActiveModSlot = EModSlots::Ve_DEFAULT;

	/** The players current health */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category= "Inventory | Player Settings")
	float CurrentPlayerHealth;

	/** The players max health */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category= "Inventory | Player Settings")
	float MaxPlayerHealth = 100.f;

	/** So that we can see in editor what our active mod's ammo is */
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category= "Inventory")
	FString ActiveModAmmoCount;

	/** The name of the character actor that the inventory is attached to */
	FString OwnerName;

	/** A pickup sound for when a valid mod gets added to the inventory
	 *Potential for various different sounds. Only one at the moment.
	 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category= "Inventory | Sound Settings")
	USoundBase* SuccessfulPickupSound;

	/** A pickup sound for when we have a full inventory. */
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Inventory | Sound Settings")
	USoundBase* FailedPickupSound;

};
