// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponSystem/ProjectileParent.h"
#include "ShotgunProjectile.generated.h"

/**
 * 
 */
UCLASS()
class BLAST_API AShotgunProjectile : public AProjectileParent
{
	GENERATED_BODY()
	public:
	AShotgunProjectile();
	/**
    *Ran Every Frame and Execute any code in this function every frame.
    *@param DeltaTime the amount of time that has passed since the last frame to use in calculations
    */
    virtual void Tick(float DeltaTime) override;
    /** called when projectile overlaps something 
    * @param OverlappedComponent component that overlapped
    * @param OtherActor other actor that was overlapped
    * @param OtherComp other component that overlapped
    * @param OtherBodyIndex index the other object is
    * @param bFromSweep if theres a sweep
    * @param SweepResult hit result from the sweep
    */
    
    virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;
    /**
    *Setter for the  bullet spread 
    *@param NewSpread New bullet spread vector
    */
    void SetSpread(FVector NewSpread);
    /**
    *Getter for the bullet spread 
    */
    FVector GetSpread() const;
    /**
    *Setter for the  bullet spread speed
    *@param NewGrowSpeed New bullet spread vector
    */
    void SetGrowSpeed(float NewGrowSpeed);
    /**
    *Getter for the bullet spread speed
    */
    float GetGrowSpeed() const;
	protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Settings | Mod Settings | Shotgun Unique", meta = (AllowProtectedAccess = "true"))
    FVector Spread;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Settings | Mod Settings | Shotgun Unique", meta = (AllowProtectedAccess = "true"))
    float GrowSpeed;
};
