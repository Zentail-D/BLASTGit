// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/AIEnemyParent.h"
#include "AI/Flying/Navigation/NavigationVolumeForAI3D.h"
#include "FlyingEnemyAI.generated.h"

/**
 * 
 */
UCLASS()
class  AFlyingEnemyAI : public AAIEnemyParent
{
	GENERATED_BODY()

public:
	/** sets up the values for this characters properties */
	AFlyingEnemyAI();

	/** the enum that has all of the attacking states for the enemy */
	enum AttackingStates {Passive, Agro, CanFire, ChargingAttack};

	/** Reference to the NavVolume that this AI will be patrolling inside of */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category= "AI Settings | Flying Settings | Movement Settings")
	ANavigationVolumeForAI3D* NavVolume3D;

	/** The speed that the AI cannot go below */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Debug Settings", meta = (AllowProtectedAccess = "true"))
	bool ShowDebug = false;

	/** the variable that actually handles the attacking state for the enemy */
	AttackingStates CurrentAttackState;
	
	/** the index for the currently target player */
	int TargetedPlayerIndex;

	/** responsible for letting us know if we are charging an attack or not
	 * @return True if not charging or not in CanFire, false if charging or CanFire
	 */
	bool CanMove() const;

	/** responsible for letting us know if we are able to shoot
	 * @return True if state is CanFire, false otherwise
	 */
	bool CanFireAttack() const;

	/** called after enemy fires to reset back to being agro */
	inline void ResetAttackStateToAgro() { CurrentAttackState = AttackingStates::Agro; }

	/** called every frame */
	virtual void Tick(float DeltaSeconds) override;

	/** For the moment just calls the parent fire (uses targetedplayerindex to fire at that player*/
	void FlyingAIFire();
	
	/** GETTERS **/

	/** @return gives the force to apply when the flying reaches a navigation path when patrolling */
	inline float GetForceToApplyWhenPatrolling() const { return ForceToApplyWhenPatrolling; }

	/** @return gives the force to apply when the flying ai reaches a navigation point when attacking */
	inline float GetForceToApplyWhenAttacking() const { return ForceToApplyWhenAttacking; }

	/** @return  gives the move to tolerance declaired in the AIEnemyParent.h */
	inline float GetMoveToTolerance() const { return MoveToTolerance; }

	/** @return gives the distance to keep from the player when hes attacking */
	inline float GetDistanceToKeepFromPlayer() const { return DistanceToKeepFromPlayer; }

	/** @return returns the min height above ground the enemy will lower to */
	inline float GetMinHeightAboveGroundWhenAttacking() const { return MinHeightAboveGroundWhenAttacking; }

	/** @return gets the max height above ground the enemy well go to */
	inline float GetMaxHeightAboveGroundWhenAttacking() const { return MaxHeightAboveGroundWhenAttacking; }

	/** @return how much time the enemy should take when charging up to attack */
	inline float GetAttackChargeUpTime() const { return AttackChargeUpTime; }

	/** @return how long currently the enemy has been charging its attack (cannot be const) */
	inline float GetCurrentAttackChargeUpTime() const { return CurrentAttackChargeUpTime; }

	/** SETTERS **/
	
	/** setter for the navigation volume that the AI will use */
	inline void SetNavigationVolume(ANavigationVolumeForAI3D* Volume) { NavVolume3D = Volume; }

	/** Just uses the patrolling linear and angular dampening variables and sets them with the capsule components */
	void SetPatrollingDampenings();
	
	/** Just uses the attacking linear and angular dampening variables and sets them with the capsule components */
	void SetAttackingDampenings();
	

protected:
	/** called once game starts running, only called once */
	virtual void BeginPlay() override;

	/** How much force is to be applied when the AI reached a navigation node */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Flying Settings | Movement Settings", meta = (AllowProtectedAccess = "true"))
	float ForceToApplyWhenPatrolling = 25000.0f;

	/** The Linear Dampining for the enemy when he is patrolling  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Flying Settings | Movement Settings", meta = (AllowProtectedAccess = "true"))
	float PatrollingLinearDampening = 1.0f;

	/** The angular dampening (for rotations) for the enemy when its patrolling  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Flying Settings | Movement Settings", meta = (AllowProtectedAccess = "true"))
	float PatrollingAngularDampening = 1.0f;

	/** How much force is to be applied when the AI reached a navigation node */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Flying Settings | Movement Settings", meta = (AllowProtectedAccess = "true"))
	float ForceToApplyWhenAttacking = 70000.0f;

	/** The Linear Dampining for the enemy when he is patrolling  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Flying Settings | Movement Settings", meta = (AllowProtectedAccess = "true"))
	float AttackingLinearDampening = 1.0f;

	/** The angular dampening (for rotations) for the enemy when its patrolling  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Flying Settings | Movement Settings", meta = (AllowProtectedAccess = "true"))
	float AttackingAngularDampening = 1.0f;

	/** How far away the enemy will keep itself from player when attacking */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Flying Settings | Movement Settings", meta = (AllowProtectedAccess = "true"))
	float DistanceToKeepFromPlayer = 1200.0f;

	/** The Min height above the ground the enemy will keep above  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Flying Settings | Movement Settings", meta = (AllowProtectedAccess = "true"))
	float MinHeightAboveGroundWhenAttacking = 350.0f;

	/** The Max height above the ground the enemy will keep above when attacking  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Flying Settings | Movement Settings", meta = (AllowProtectedAccess = "true"))
	float MaxHeightAboveGroundWhenAttacking = 600.0f;

	/** The Max height above the ground the enemy will keep above when attacking  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Flying Settings | Attack Settings", meta = (AllowProtectedAccess = "true"))
	float AttackChargeUpTime = 3.0f;

	/** What the current attack charge up time is */
	float CurrentAttackChargeUpTime;

private:
	/**
	 * method that I will be using to decremeant my charge up cooldown
	 * @param DeltaTime - time to decrease chareupTime by, usually DeltaTime
	 */
	inline void DecreaseChargeUpTime(float DeltaTime) { CurrentAttackChargeUpTime -= DeltaTime; }

	/**
	 *method called when we want to very readibly reset our charge up time sets CurrentAttackChargeUpTime = AttackChargeUpTime; */
	inline void ResetChargeUpTime() { CurrentAttackChargeUpTime = AttackChargeUpTime; }

};
