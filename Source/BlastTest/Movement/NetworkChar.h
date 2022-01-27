// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "InventoryComponent.h"
#include "GameFramework/Character.h"
#include "ZoneTool/ZoneToolCon.h"
#include "NetworkChar.generated.h"


UCLASS(Blueprintable)
class ANetworkChar : public ACharacter
{
	GENERATED_BODY()
	
public:
	
	/** reference to the player inventory component */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UInventoryComponent* PlayerInventory;

	/**Force the player to respawn by clicking button in editor*/
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Respawn")
	void ForceRespawnPlayer();
	
	/**
	 *Sets default values for this character's properties
	 * @param ObjectInitializer 
	 */
	ANetworkChar(const class FObjectInitializer& ObjectInitializer);
	
	/**
	 * Called every frame
	 * @param DeltaTime The change in time
	 */
	virtual void Tick(float DeltaTime) override;

	/**
	 * method to call when the player dies and is responsible for Reset the player location and stats
	 */
	UFUNCTION(BlueprintCallable)
	void RespawnPlayer();

	/** decreases the current health by damage to deal amount
	 * @return True if the damage dealt killed the player, false if not
	 */
	UFUNCTION(BlueprintCallable)
	bool DealDamageToPlayer(float DamageToDeal);

protected:	
	/* Called when the game starts or when spawned **/
	virtual void BeginPlay() override;

	virtual void Destroyed() override;
	
	/**Class that the projectiles are fired using*/
	UPROPERTY(EditAnywhere, Category = "Network Char | Player Start", meta= (AllowPrivateAccess= "true"))
	TSubclassOf<class AActor> PlayerStart;

	UFUNCTION(BlueprintCallable)
	void RemoveFromZone();
	
private:
	
	/**Reference to the ZoneToolCon*/
	AZoneToolCon* ZoneCon;
	
};





