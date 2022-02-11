// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "GameFrameWork/ProjectileMovementComponent.h"
#include "ProjectileParent.generated.h"

UCLASS()
class AProjectileParent : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileParent();

	/**
	*Ran Every Frame and Execute any code in this function every frame.
	*@param DeltaTime the amount of time that has passed since the last frame to use in calculations
	*/
	virtual void Tick(float DeltaTime) override;

	/**
	 * Collision Component for the bullet initialized and overidden to be the root scene component
	 */
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	UBoxComponent* CollisionComponent;

	/**
	* Projectile Move Component that handles causing the bullet to move at a certain velocity
	*/
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = Movement)
	UProjectileMovementComponent* ProjectileMovementComponent;

	class ANetworkChar* OwningPlayer;
	
	/**
	 * Turns the debug box for the collision component on and off
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug Tools", meta = (AllowProtectedAccess = "true"))
	bool bShowDebugBox;
	/**
	* Fires a projectile in the Direction of ShootDirection and at the initial speed in The ProjectileMovementComponent
	* @param ShootDirection Direction the bullet needs to travel in
	*/
	void FireInDirection(const FVector& ShootDirection) const;
	
	/** called when projectile overlaps something 
	* @param OverlappedComponent component that overlapped
	* @param OtherActor other actor that was overlapped
	* @param OtherComp other component that overlapped
	* @param OtherBodyIndex index the other object is
	* @param bFromSweep if theres a sweep
	* @param SweepResult hit result from the sweep
	*/
	UFUNCTION()
	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	/**
	 *Setter for the Damage that the bullet does
	 *@param NewDamageAmount New Amount of damage the bullet needs to do
	*/
	void SetDamageAmount(int NewDamageAmount);

	int GetDamageAmount() const;
	/**
	*Setter for the Impulse that the bullet does
	*@param NewImpulsePower New Power of impulse the bullet needs to do
	*/
	void SetImpulsePower(int NewImpulsePower);

	int GetImpulsePower() const;

	/** call when spawning a projectile if you want to use this to set the lifespan
	 *  @param Lifespan - the new lifespan to set to
	 */
	inline void SetProjectileLifespan(const float Lifespan) { ProjectileLifespan = Lifespan; }

	/** setter to be called when a projectile is created so that we can set the owner so it doesnt damage itself */
	void inline SetOwnersName(FString Name) { OwnerName = Name; }

protected:

	/**
	 * Amount of damage the projectile does
	 */
	UPROPERTY(EditAnywhere, Category = "Projectile Settings", meta= (AllowProtectedAccess= "true"))
	int DamageAmount;

	UPROPERTY(EditAnywhere, Category = "Projectile Settings", meta= (AllowProtectedAccess= "true"))
	float ProjectileLifespan;
	UPROPERTY(EditAnywhere, Category = "Projectile Settings", meta= (AllowProtectedAccess= "true"))
	float ImpulsePower;
	
	/** Name of the players character that has fired this projectile */
	FString OwnerName;

};
