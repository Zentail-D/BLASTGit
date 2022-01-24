// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFrameWork/ProjectileMovementComponent.h"
#include "EnemyProjectile.generated.h"

UCLASS()
class  AEnemyProjectile : public AActor
{
	GENERATED_BODY()
	
	public:	
	// Sets default values for this actor's properties
	AEnemyProjectile();

	/**
	*Ran Every Frame and Execute any code in this function every frame.
	*@param DeltaTime the amount of time that has passed since the last frame to use in calculations
	*/
	virtual void Tick(float DeltaTime) override;

	/**
	 * Collision Component for the bullet initialized and overidden to be the root scene component
	 */
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComponent;

	/**
	* Projectile Move Component that handles causing the bullet to move at a certain velocity
	*/
	UPROPERTY(VisibleAnywhere, Category = Movement)
	UProjectileMovementComponent* ProjectileMovementComponent;

	/**
	* Fires a projectile in the Direction of ShootDirection and at the initial speed in The ProjectileMovementComponent
	* @param ShootDirection Direction the bullet needs to travel in
	*/
	void FireInDirection(const FVector& ShootDirection);


	/**
	* Function is called when collision component hits something
	* @param HitComponent PrimitiveComponent that the collision sphere hits
	* @param OtherActor Reference to the actor hit by the collision sphere
	* @param OtherComponent Reference to the Component Hit by the collision sphere
	* @param NormalImpulse Vector to the Direction of the impulse for the collison
	* @param Hit This is where all the information about the hit is stored. Blueprints this would be where you split the HitResult
	* @warning Linked with the collision component. If the collision component doesnt collision enabled this wont be called
	*/
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
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

	protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**
	 * Amount of damage the projectile does
	 */
	int DamageAmount;
	/**
	* deals damage to the health variable of the collided object
	* @param Health current health of the object 
	* @param Damage  amount of damage being done 
	* @param MaxHealth max health of the object
	* @return new health of the object
	*/
	static float DealDamage(float Health,float Damage,float MaxHealth);

};
