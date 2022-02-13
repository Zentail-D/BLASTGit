// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/AIEnemyParent.h"
#include "GruntAIEnemy.generated.h"

/**
 * 
 */
UCLASS()
class  AGruntAIEnemy : public AAIEnemyParent
{
	GENERATED_BODY()

public:
	/** constructor that just runs when ai is made */
	AGruntAIEnemy();

	/** boolean to tell the grunt to fire*/
	bool bFireBurst=false;

	/** boolean to tell the grunt it has fire queued*/
	bool bQueuedFire=false;

	/** boolean to tell the grunt it fire has completed the queue and is ready to fire a burst*/
	bool bReadyToFire=true;

	/** The angle range between for the strafing (note: if you put 45 the range will be 90 degress */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Debug Settings", meta = (AllowProtectedAccess = "true"))
	bool ShowDebug = false;

	/** Method that gets called when the game is ran */
	virtual void BeginPlay() override;

	/** The grunt version of fire, just calls the parent fire with an index check */
	void GruntAIFire(int TargetedPlayerIndex);

	/** SETTERS */

	/** Sets the current attacking destination */
	inline void SetCurrentAttackingDestination(const FVector Location) { CurrentAttackingDestination = Location; }

	/** sets if we need a new attack destination */
	inline void SetNeedNewAttackDestination(const bool NeedNew) { NeedNewAttackingDestination = NeedNew; }

	/** sets the target player index */
	inline void SetCurrentTargetPlayerIndex(const int Index) { CurrentTargetPlayerIndex = Index; }

	/** GETTERS */
	
	/** Gives the Patrol point radius tolerance */
	inline float GetPatrolPointRadiusTolerance() const { return MoveToTolerance; }

	/** Returns the strafing sharpness angle */
	inline float GetStrafingSharpnessAngle() const { return StrafingSharpnessAngle; }

	/** returns the strafing variation angle */
	inline float GetStrafingVariationAngle() const { return StrafingVariationAngle; }

	/** returns the distance for each move too during strafing */
	inline float GetStrafingDistance() const { return StrafingDistance; }

	/** returns the strafing subsection for the path to a location when strafing*/
	inline float GetStrafingSubsection() const {return StrafingSubsection; }
	
	/** returns the point that the attacking is currently moving to or wants to move to */
	inline FVector GetCurrentAttackingDestination() const { return CurrentAttackingDestination; }

	/** returns if we need a new attack destination or not */
	inline bool GetNeedNewAttackDestination() const { return NeedNewAttackingDestination; }

	/** gets the current target player index
	 * @return - the index of targeted player (will be -1 if no player to target)
	 */
	inline int GetCurrentTargetPlayerIndex() const { return CurrentTargetPlayerIndex; }
	
	/**
	*Set a TArray of Vectors to the strafing path
	*/
	inline void SetStrafingPath(const TArray<FVector> NewStrafingPath) {StrafingPath=NewStrafingPath; StrafingPathIndex=0; }

	/**
	 * Increment the StrafingPathIndex if we are incrementing above the StrafingPath Len we return true for we are finish
	 * @returns True if we are at the end of the path and false if we need to continue incrementing
	 */
	inline bool IncrementStrafingPathIndex() {StrafingPathIndex++; return StrafingPathIndex>=StrafingPath.Num();}

	/**
	 * Get CurrentStrafingLocation for the Grunt
	 */
	inline FVector GetCurrentStrafingLocation() {return StrafingPath[StrafingPathIndex];}

protected:
	UPROPERTY(VisibleDefaultsOnly, Category= "Projectile")
	UStaticMeshComponent* AMuzzleLocation;

	/** Array holding all the points that are contained in a strafe for the grunt. Amount of points are directly tied to the strafing subsections*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Grunt Settings | Attack Settings", meta = (AllowProtectedAccess = "true"))
	TArray<FVector> StrafingPath;

	/** Index at which the Strafing is at*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Grunt Settings | Attack Settings", meta = (AllowProtectedAccess = "true"))
	int StrafingPathIndex=0;
	
	/** How many points will be created between the end goal and the start for the grunt when strafing*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Grunt Settings | Attack Settings", meta = (AllowProtectedAccess = "true", ClampMin=2))
	int StrafingSubsection = 3;
	
	/** Same as strafe sharpness angle but for strafing */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Grunt Settings | Attack Settings", meta = (AllowProtectedAccess = "true"))
	float StrafingSharpnessAngle = 85.0f;

	/** Same as variance angle but for strafing */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Grunt Settings | Attack Settings", meta = (AllowProtectedAccess = "true"))
	float StrafingVariationAngle = 10.0f;

	/** How far the enemy will run in the chosen direction when strafing */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Grunt Settings | Attack Settings", meta = (AllowProtectedAccess = "true"))
	float StrafingDistance = 400.0f;

	/** How long to wait between shots during burst fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Grunt Settings | Attack Settings", meta = (AllowProtectedAccess = "true"))
	float TimeBetweenShots = 0.2f;

	float TimeBetweenShotsReset = TimeBetweenShots;

	/** The min bullets that can be shot during a burst fire (inclusive) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Grunt Settings | Attack Settings", meta = (AllowProtectedAccess = "true"))
	int MinBulletsToFire = 2;

	/** The Max number of bullets that can be shot during a burst fire (inclusive) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Grunt Settings | Attack Settings", meta = (AllowProtectedAccess = "true"))
	int MaxBulletsToFire = 5;

	/** The point that the enemy is currently moving to while attacking */
	FVector CurrentAttackingDestination;

	bool NeedNewAttackingDestination = true;

	/** the index into the player array for the current target */
	int CurrentTargetPlayerIndex;

	/** how many bullets are in current burst */
	int BulletsInBurst;

	/** when true it means we re calculate the number of bullets in burst */
	bool CalculateBulletsInBurst = true;

	/** the tick function that gets called every frame
	 * @param DeltaSeconds - time since last frame
	 */
	virtual void Tick(float DeltaSeconds) override;

	/** decrements the current attack cooldown
	 * @param DeltaTime - The ammount to declrement
	 * */
	inline void DecreaseAttackCooldown(const float DeltaTime) {AttackingCooldown -= DeltaTime; }

	/** Resets the current attack cooldown to the reset value */
	inline void ResetAttackCooldown() { AttackingCooldown = AttackCooldownReset; }

	/** decrements the current burst fire cooldown
	 * @param DeltaTime - how much time to decrease by
	 * */
	inline void DecreaseBurstAttackCooldown(const float DeltaTime) {TimeBetweenShots -= DeltaTime; }

	/** resets the burst attack cooldown */
	inline void ResetBurstAttackCooldown() {TimeBetweenShots = TimeBetweenShotsReset; }
};
