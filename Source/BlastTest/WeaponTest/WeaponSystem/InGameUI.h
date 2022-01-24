// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameUI.generated.h"

/**
 * 
 */
UCLASS()
class UInGameUI : public UUserWidget
{
	GENERATED_BODY()
	public:
	/**
	*Sets default values for widgets properties
	* @param ObjectInitializer 
	*/
	UInGameUI(const FObjectInitializer& ObjectInitializer);
	/**
	 * is like BeginPlay for actors/characters, 
	 */
	virtual void NativeConstruct() override;
	//ammo/weapons
	/**text block displaying ammo count 1*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* AmmoSlotOne;
	/**text block displaying ammo count 2*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* AmmoSlotTwo;
	/**Progress Bar  displaying charge amount (if applies*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* ChargeTime;
	/**text block displaying current health*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* CrossHair;
	/**holds texture for croshair*/
	UTexture2D* CrossHairTexture;
	/**
	* updates all Health Ui objects
	* @param NewAmmoCount updated ammo count 
	* @param bActiveAmmo what ammo slot to update(if true, active is AmmoOne, if false, AmmoTwo)
	*/
	void UpdateAmmoUI(float NewAmmoCount,int bActiveAmmo);
	/**
	* updates Charging bar 
	* @param NewChargeTime updated charge time 
	*/
	void UpdateChargeUI(float NewChargeTime,float MaxChargeTime);
	/**
	* updates Charging bar 
	*/
	void NotCharging();
	
	//health
	/**text block displaying current health*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* CurrentHealth;
	/**Progress Bar  displaying Health*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* Health;
	/**text block displaying a '/'*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* OutOf;
	/**text block displaying Max health*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* MaxHealth;
	/**
	 * updates all Health Ui objects
	 * @param newCurrentHealth updated current health
	 * @param newMaxHealth updated max health
	 */
	void UpdateHealthUI(float newCurrentHealth,float newMaxHealth);
};
