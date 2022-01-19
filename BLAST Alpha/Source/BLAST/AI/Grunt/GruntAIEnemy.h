// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/AIEnemyParent.h"
#include "GruntAIEnemy.generated.h"

/**
 * 
 */
UCLASS()
class BLAST_API AGruntAIEnemy : public AAIEnemyParent
{
	GENERATED_BODY()

public:
	/** constructor that just runs when ai is made */
	AGruntAIEnemy();

	/** The enum class for the attacking states that the enemy can be in */
	enum AttackStates {Patrolling, Approaching, Strafing, Shooting};

	/** true if the grunt is moving to a point during attack */
	bool IsGruntMoving = false;

	/** The angle range between for the strafing (note: if you put 45 the range will be 90 degress */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Debug Settings", meta = (AllowProtectedAccess = "true"))
	bool ShowDebug = false;

	/** Method that gets called when the game is ran */
	virtual void BeginPlay() override;

	/** The grunt version of fire, just calls the parent fire with an index check */
	void GruntAIFire(int TargetedPlayerIndex);

	/** SETTERS */

	/** Set the current attack state for the enemy */
	inline void SetAttackingState(const AttackStates State) { CurrentAttackState = State; }

	/** Sets the current attacking destination */
	inline void SetCurrentAttackingDestination(const FVector Location) { CurrentAttackingDestination = Location; }

	/** sets if we need a new attack destination */
	inline void SetNeedNewAttackDestination(const bool NeedNew) { NeedNewAttackingDestination = NeedNew; }

	/** sets the target player index */
	inline void SetCurrentTargetPlayerIndex(const int Index) { CurrentTargetPlayerIndex = Index; }

	/** GETTERS */

	/** gives the current attack state of the grunt */
	inline AttackStates GetCurrentAttackState() const { return CurrentAttackState; }
	
	/** Gives the Patrol point radius tolerance */
	inline float GetPatrolPointRadiusTolerance() const { return PatrolPointRadiusTolerance; }

	/** returns the distance to keep from the player (roughly) */
	inline float GetDistanceToKeepFromPlayer() const { return DistanceToKeepFromPlayer; }
	
	/** Gives the StrafeSharpnessAngle */
	inline float GetApproachSharpnessAngle() const { return ApproachSharpnessAngle; }

	/** Returns the angle that represents the variation in where the player will move to once the strafe direction is determined */
	inline float GetApproachVariationAngle() const { return ApproachVariationAngle; }

	/** returns the distance between each zig zag approach point */
	inline float GetApproachDistance() const { return ApproachDistance; }

	/** Returns the strafing sharpness angle */
	inline float GetStrafingSharpnessAngle() const { return StrafingSharpnessAngle; }

	/** returns the strafing variation angle */
	inline float GetStrafingVariationAngle() const { return StrafingVariationAngle; }

	/** returns the distance for each move too during strafing */
	inline float GetStrafingDistance() const { return StrafingDistance; }
	
	/** returns the point that the attacking is currently moving to or wants to move to */
	inline FVector GetCurrentAttackingDestination() const { return CurrentAttackingDestination; }

	/** returns if we need a new attack destination or not */
	inline bool GetNeedNewAttackDestination() const { return NeedNewAttackingDestination; }

	/** gets the current target player index
	 * @return - the index of targeted player (will be -1 if no player to target)
	 */
	inline int GetCurrentTargetPlayerIndex() const { return CurrentTargetPlayerIndex; }

protected:
	UPROPERTY(VisibleDefaultsOnly, Category= "Projectile")
	UStaticMeshComponent* AMuzzleLocation;
	
	/** Radius around each patrol point that it will find a point to move to somewhere within the circle made by the radius */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Grunt Settings | Movement Settings", meta = (AllowProtectedAccess = "true"))
	float PatrolPointRadiusTolerance = 200.0f;

	/** The distance that the enemy will stop approaching at and start strafing */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Grunt Settings | Attack Settings", meta = (AllowProtectedAccess = "true"))
	float DistanceToKeepFromPlayer = 500.0f;

	/** Angle that determines how sharp the strafe will be (how hard the enemy turns) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Grunt Settings | Attack Settings", meta = (AllowProtectedAccess = "true"))
	float ApproachSharpnessAngle = 45.0f;

	/** Angle that the slight variation in where the player moves to once its strafe direction is found */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Grunt Settings | Attack Settings", meta = (AllowProtectedAccess = "true"))
	float ApproachVariationAngle = 10.0f;

	/** The distance for each move to while approaching */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Grunt Settings | Attack Settings", meta = (AllowProtectedAccess = "true"))
	float ApproachDistance = 175.0f;
	
	/** Same as strafe sharpness angle but for strafing */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Grunt Settings | Attack Settings", meta = (AllowProtectedAccess = "true"))
	float StrafingSharpnessAngle = 85.0f;

	/** Same as variance angle but for strafing */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Grunt Settings | Attack Settings", meta = (AllowProtectedAccess = "true"))
	float StrafingVariationAngle = 10.0f;

	/** How far the enemy will run in the chosen direction when strafing */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Grunt Settings | Attack Settings", meta = (AllowProtectedAccess = "true"))
	float StrafingDistance = 300.0f;

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

	/** The current attacking state, initialize to patrolling */
	AttackStates CurrentAttackState = AttackStates::Patrolling;

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
